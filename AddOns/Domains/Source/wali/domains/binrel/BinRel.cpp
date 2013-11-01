/**
 * @author Nicholas Kidd
 * @author Prathmesh Prabhu
 * @version $Id: BinRel.cpp 557 2009-10-19 18:51:54Z kidd $
 */
 
#include "BinRel.hpp"
#include "buddy/fdd.h"
#include "ProgramBddContext.hpp"
//#include "BuddyExt.hpp"
#include "combination.hpp"

#include <iostream>
#include <sstream>
#include <cmath>

#include <boost/algorithm/string/predicate.hpp>

#include <tr1/unordered_map>

using namespace wali::domains::binrel;
using std::endl;
using wali::waliErr;
using std::cout;

// ////////////////////////////
// For large memory machines consider allocating ~1500 MB
// 25000000 ~~ 500 MB, change the multiple to get what you need.
#define FIVE_HUNDRED_MB 25000000
#define BDDMEMSIZE (FIVE_HUNDRED_MB*1)
// Find out emperically what the best ratio of memsize to cache size is
#define MEMTOCACHE 10

#define MAXMEMINC BDDMEMSIZE/10

//It's a good habit to forward declare all the static functions in the
//file so that there is an index and so that the contents of the file can
//move around freely.

namespace wali
{
  namespace domains
  {
    namespace binrel
    {
      /**
       * idx2Name gives strings to print for bdd level indices.
       * This is passed to the callback function in buddy.
       * This is currently a global so that it can be 
       * accessed in BinRelManager.
       **/
      RevBddContext idx2Name;

      static void myFddStrmHandler(std::ostream &o, int var);
      static BinRel* convert(wali::SemElem* se);


      /*
      struct BddLessThan
      {
        bool operator() (bdd left, bdd right) const {
          return left.id() < right.id();
        }
      };
      std::map<bdd, sem_elem_t, BddLessThan> star_cache;
      */

      typedef std::pair< bdd, bool> StarCacheKey;
      struct StarCacheHash
      {
        size_t operator() (StarCacheKey k) const
        {
          return k.first.id() << 1 & (int) k.second;
        }
      };
      std::tr1::unordered_map< StarCacheKey, sem_elem_t, StarCacheHash> star_cache;


      namespace details {

        bool
        bddImplies_using_bdd_imp(bdd left, bdd right)
        {
          // left_implies_right(x) = (left(x) => right(x))
          bdd left_implies_right = bdd_imp(left, right);
          return left_implies_right == bddtrue;
        }
        
        bool
        bddImplies_recursive(bdd left, bdd right)
        {
          if (left == right
              || left == bddfalse
              || right == bddtrue)
          {
            return true;
          }

          if (left == bddtrue || right == bddfalse) {
            return false;
          }

          // Both are nontrivial. But the root of one might be different than
          // the root of another...
          int
            left_varno  = bdd_var(left),
            right_varno = bdd_var(right),
            left_level  = bdd_var2level(left_varno),
            right_level = bdd_var2level(right_varno);

          if (left_level < right_level) {
            // Left's root is higher, so we look at left's children
            bool left_good = bddImplies_recursive(bdd_low(left), right);
            bool right_good = bddImplies_recursive(bdd_high(left), right);
            return left_good && right_good;
          }
          else if (left_level > right_level) {
            // Right's root is higher, so we look at right's children
            bool left_good = bddImplies_recursive(left, bdd_low(right));
            bool right_good = bddImplies_recursive(left, bdd_high(right));
            return left_good && right_good;
          }
          else {
            // The root is the same
            bool left_good = bddImplies_recursive(bdd_low(left), bdd_low(right));
            bool right_good = bddImplies_recursive(bdd_high(left), bdd_high(right));
            return left_good && right_good;
          }
        }
        
        
        /// Returns if 'left(x_vec) => right(x_vec)', aka if 'left' is a
        /// superset of 'right' (when viewed as sets of accepting 'x_vec's.
        bool
        bddImplies(bdd left, bdd right)
        {
          bool fast_answer = bddImplies_recursive(left, right);
#if CHECK_BDDIMPLIES_WITH_SLOWER_VERSION
          bool slow_answer = bddImplies_using_bdd_imp(left, right);
          assert(fast_answer == slow_answer);
#endif
          return fast_answer;
        }
        
      }
    }
  }
}


// ////////////////////////////
// Definitions of static members from BddContext/BinRel class

int BddContext::numBddContexts = 0;
// ////////////////////////////

std::ostream& BddInfo::print(std::ostream& o) const
{
  o << "{ Bddinfo: "
    << "[maxVal " << maxVal << "] "
    << "[baseLhs " << baseLhs << "] "
    << "[baseRhs " << baseRhs << "] "
    << "[baseExtra " << baseExtra << "] "
    << "[tensor1Lhs " << tensor1Lhs << "] "
    << "[tensor1Rhs " << tensor1Rhs << "] "
    << "[tensor1Extra " << tensor1Extra << "] "
    << "[tensor2Lhs " << tensor2Lhs << "] "
    << "[tensor2Rhs " << tensor2Rhs << "] "
    << "[tensor2Extra " << tensor2Extra << "] } ";
  return o;
}

namespace wali
{
  namespace domains
  {
    namespace binrel
    {
      static void myFddStrmHandler(std::ostream &o, int var)
      {
        extern RevBddContext idx2Name;
        o << idx2Name[var];
      }

      // Helper function that converts a SemElem
      // into a BinRel*
      static BinRel* convert(wali::SemElem* se) 
      {
        BinRel* br = dynamic_cast<BinRel*>(se);
        if (br == NULL) {
          *waliErr << "[ERROR] Cannot cast to class wali::binrel::BinRel.\n";
          se->print( *waliErr << "    " ) << endl;
          assert(false);
        }
        // When done with developement
        // BinRel* br = static_cast<BinRel*>(se)
        return br;
      }
    } // namespace binrel
  } // namespace domains
} // namespace wali


BddContext::BddContext(int bddMemSize, int cacheSize) :
  std::map< const std::string, bddinfo_t>(),
  count(0)
{
  //If buddy has not been initialized, initialize it.
  //We handle this by keeping track of the number of BddContext objects
  //lying around. Since every BinRel also has a BddContext object in it,
  //if there are no BddContext objects, there is Nothing!
  //lock mutex
  if(numBddContexts == 0){
    // ///////////////////////
    // Begin initialize BuDDy
    bddMemSize = (bddMemSize==0)?BDDMEMSIZE:bddMemSize;
    cacheSize = (bddMemSize/MEMTOCACHE > cacheSize)? bddMemSize/MEMTOCACHE : cacheSize;
    if (0 == bdd_isrunning()){
      int rc = bdd_init(bddMemSize,cacheSize);
      if( rc < 0 ){
        *waliErr << "[ERROR] " << bdd_errstring(rc) << endl;
        assert( 0 );
      }
      // Default is 50,000 (1 Mb),memory is cheap, so use 100,000      
      bdd_setmaxincrease((bddMemSize/10 > MAXMEMINC)? bddMemSize/10 : MAXMEMINC);
      // TODO: bdd_error_hook( my_error_handler );
      fdd_strm_hook( myFddStrmHandler );
    }else{
      //can not happen, unless reset fails.
      *waliErr << "[ERROR] BuDDy already initialized." << endl;
      assert( 0 );
    }
    // End initialize BuDDy
    // ///////////////////////
  }
  numBddContexts++;
  //release mutex

  baseSwap = BddPairPtr(bdd_newpair());
  tensor1Swap = BddPairPtr(bdd_newpair());
  baseRightShift = BddPairPtr(bdd_newpair());
  tensorRightShift = BddPairPtr(bdd_newpair());
  baseRestore = BddPairPtr(bdd_newpair());
  tensorRestore = BddPairPtr(bdd_newpair());
  move2Tensor1 = BddPairPtr(bdd_newpair());
  move2Tensor2 = BddPairPtr(bdd_newpair());
  move2Base = BddPairPtr(bdd_newpair());
  move2BaseTwisted = BddPairPtr(bdd_newpair());
#if (NWA_DETENSOR == 1)
  rawMove2Tensor2 = BddPairPtr(bdd_newpair());
#endif

  //initialize static bdds
  baseSecBddContextSet = bddtrue;
  tensorSecBddContextSet= bddtrue;
  commonBddContextSet23 = bddtrue;
  commonBddContextSet13 = bddtrue;
  commonBddContextId23 = bddtrue;
  commonBddContextId13 = bddtrue;

  cachedBaseOne = NULL;
  cachedBaseZero = NULL;
  cachedBaseTop = NULL;
  cachedTensorOne = NULL;
  cachedTensorZero = NULL;
  cachedTensorTop = NULL;

#ifdef BINREL_STATS
  numCompose = 0;
  numUnion = 0;
  numIntersect = 0;
  numEqual = 0;
  numKronecker = 0;
  numReverse = 0;
  numTranspose = 0;
  numDetensor = 0;
  numDetensorTranspose = 0;
#endif
  populateCache();
}

//Don't copy count
BddContext::BddContext(const BddContext& other) :
  std::map< const std::string, bddinfo_t>(other),
  count(0),
  baseSwap(other.baseSwap),
  tensor1Swap(other.tensor1Swap),
  baseRightShift(other.baseRightShift),
  tensorRightShift(other.tensorRightShift),
  baseRestore(other.baseRestore),
  tensorRestore(other.tensorRestore),
  move2Tensor1(other.move2Tensor1),
  move2Tensor2(other.move2Tensor2),
  move2Base(other.move2Base),
  move2BaseTwisted(other.move2BaseTwisted),
  baseSecBddContextSet(other.baseSecBddContextSet),
  tensorSecBddContextSet(other.tensorSecBddContextSet),
  commonBddContextSet23(other.commonBddContextSet23),
  commonBddContextSet13(other.commonBddContextSet13),
  commonBddContextId13(other.commonBddContextId13),
  cachedBaseOne(other.cachedBaseOne),
  cachedBaseZero(other.cachedBaseZero),
  cachedTensorOne(other.cachedTensorOne),
  cachedTensorZero(other.cachedTensorZero)
{
  numBddContexts++;

#ifdef BINREL_STATS
  // Althought we copied contexts, the statistics are independent.
  numCompose = 0;
  numUnion = 0;
  numIntersect = 0;
  numEqual = 0;
  numKronecker = 0;
  numReverse = 0;
  numTranspose = 0;
  numDetensor = 0;
  numDetensorTranspose = 0;
#endif
  populateCache();
}

BddContext& BddContext::operator = (const BddContext& other)
{
  if(this!=&other){
    count=0;
    baseSwap=other.baseSwap;
    tensor1Swap = other.baseSwap;
    baseRightShift=other.baseRightShift;
    tensorRightShift=other.tensorRightShift;
    baseRestore=other.baseRestore;
    tensorRestore=other.tensorRestore;
    move2Tensor1=other.move2Tensor1;
    move2Tensor2=other.move2Tensor2;
    move2Base=other.move2Base;
    move2BaseTwisted=other.move2BaseTwisted;
    baseSecBddContextSet=other.baseSecBddContextSet;
    tensorSecBddContextSet=other.tensorSecBddContextSet;
    commonBddContextSet23=other.commonBddContextSet23;
    commonBddContextSet13=other.commonBddContextSet13;
    commonBddContextId13=other.commonBddContextId13;
    cachedBaseOne=other.cachedBaseOne;
    cachedBaseZero=other.cachedBaseZero;
    cachedTensorOne=other.cachedTensorOne;
    cachedTensorZero=other.cachedTensorZero;
    populateCache();
  }
  return *this;
}


BddContext::~BddContext()
{
  // shared_ptr::reset sets it to NULL
  baseSwap.reset();
  tensor1Swap.reset();
  baseRightShift.reset();
  tensorRightShift.reset();
  baseRestore.reset();
  tensorRestore.reset();
  move2Tensor1.reset();
  move2Tensor2.reset();
  move2Base.reset();
  move2BaseTwisted.reset();
#if (NWA_DETENSOR == 1)
  rawMove2Tensor2.reset();
#endif
  
  baseSecBddContextSet = bddtrue;
  tensorSecBddContextSet = bddtrue;
  commonBddContextSet23 = bddtrue;
  commonBddContextId23 = bddtrue;
  commonBddContextSet13 = bddtrue;
  commonBddContextId13 = bddtrue;

  //Delete cached BinRel objects.
  cachedBaseOne = NULL;
  cachedBaseZero = NULL;
  cachedBaseTop = NULL;
  cachedTensorOne = NULL;
  cachedTensorZero = NULL;
  cachedTensorTop = NULL;

  //Clear the bddinfo_t vector.
  this->clear();

  //lock mutex
  numBddContexts--;
  if(numBddContexts == 0){
    //All BddContexts are now dead. So we must shutdown buddy.
    star_cache.clear();
    if(bdd_isrunning() != 0)
      bdd_done();
    //Now clear the reverse map.
    idx2Name.clear();
    //Also clean up the BinRel class
    BinRel::reset();
  }
  //release mutex
}

void BddContext::addBoolVar(std::string name)
{
  addIntVar(name,2);
}

void BddContext::setIntVars(const std::map<std::string, int>& flatvars)
{
  std::vector<std::map<std::string, int> > vars;
  vars.push_back(flatvars);
  setIntVars(vars);
}

void BddContext::createIntVars(const std::vector<std::map<std::string, int> >& vars)
{
  int vari;
  // First work through the variable list and create the vocabulary structure
  // This will collect information about the fdds to be created in buddy
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    for(std::map<std::string, int>::const_iterator cmi = (*cvi).begin(); cmi != (*cvi).end(); ++cmi){
      if(cmi->second < 2){
        *waliErr << "I haven't tested the library for int size less than 2";
        assert(false);
      }
      bddinfo_t varInfo = new BddInfo;
      varInfo->maxVal = cmi->second;
      (*this)[cmi->first] = varInfo;
    }
  }

#if (TENSOR_MAX_AFFINITY == 1)
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    int * domains = new int[9 * interleavedVars.size()];
    vari = 0;
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 9; j < 9 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 9 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      bddinfo_t varInfo = (*this)[cmi->first];
      int j = 9 * vari;
      varInfo->baseLhs = base + j++;
      varInfo->baseRhs = base + j++;
      varInfo->baseExtra = base + j++;
      varInfo->tensor1Lhs = base + j++;
      varInfo->tensor1Rhs = base + j++;
      varInfo->tensor1Extra = base + j++;
      varInfo->tensor2Lhs = base + j++;
      varInfo->tensor2Rhs = base + j++;
      varInfo->tensor2Extra = base + j++;
      vari++;  
    }
  }
#elif (TENSOR_MIN_AFFINITY == 1)
  //First the base levels
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator ci = interleavedVars.begin(); ci != interleavedVars.end(); ++ci){
      bddinfo_t varInfo = (*this)[ci->first];
      int j = 3 * vari;
      varInfo->baseLhs = base + j++;
      varInfo->baseRhs = base + j++;
      varInfo->baseExtra = base + j++;
      vari++;  
    }
  }
  //Next for tensor1
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator ci = interleavedVars.begin(); ci != interleavedVars.end(); ++ci){
      bddinfo_t varInfo = (*this)[ci->first];
      int j = 3 * vari;
      varInfo->tensor1Lhs = base + j++;
      varInfo->tensor1Rhs = base + j++;
      varInfo->tensor1Extra = base + j++;
      vari++;  
    }
  }
  //Finally for tensor2
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator ci = interleavedVars.begin(); ci != interleavedVars.end(); ++ci){
      bddinfo_t varInfo = (*this)[ci->first];
      int j = 3 * vari;
      varInfo->tensor2Lhs = base + j++;
      varInfo->tensor2Rhs = base + j++;
      varInfo->tensor2Extra = base + j++;
      vari++;  
    }
  }

#elif (TENSOR_MATCHED_PAREN == 1)
  //First the base levels
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator ci = interleavedVars.begin(); ci != interleavedVars.end(); ++ci){
      bddinfo_t varInfo = (*this)[ci->first];
      int j = 3 * vari;
      varInfo->baseExtra = base + j++;
      varInfo->baseRhs = base + j++;
      varInfo->baseLhs = base + j++;
      vari++;  
    }
  }
  //Next for tensor1
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator ci = interleavedVars.begin(); ci != interleavedVars.end(); ++ci){
      bddinfo_t varInfo = (*this)[ci->first];
      int j = 3 * vari;
      // In this case, tensor1 has the three levels reversed. 
      varInfo->tensor1Extra = base + j++;
      varInfo->tensor1Rhs = base + j++;
      varInfo->tensor1Lhs = base + j++;
      vari++;  
    }
  }
  //Finally for tensor2
  for(std::vector<std::map<std::string, int> >::const_reverse_iterator crvi = vars.rbegin(); crvi != vars.rend(); ++crvi){
    std::map<std::string, int> interleavedVars = *crvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_reverse_iterator crmi = interleavedVars.rbegin(); crmi != interleavedVars.rend(); ++crmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = crmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_reverse_iterator cri = interleavedVars.rbegin(); cri != interleavedVars.rend(); ++cri){
      bddinfo_t varInfo = (*this)[cri->first];
      int j = 3 * vari;
      varInfo->tensor2Lhs = base + j++;
      varInfo->tensor2Rhs = base + j++;
      varInfo->tensor2Extra = base + j++;
      vari++;  
    }
  }
#elif (BASE_MAX_AFFINITY_TENSOR_MIXED == 1)
  //First the base levels
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    vari = 0;
    int * domains = new int[3 * interleavedVars.size()];
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 3; j < 3 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 3 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator ci = interleavedVars.begin(); ci != interleavedVars.end(); ++ci){
      bddinfo_t varInfo = (*this)[ci->first];
      int j = 3 * vari;
      varInfo->baseLhs = base + j++;
      varInfo->baseRhs = base + j++;
      varInfo->baseExtra = base + j++;
      vari++;  
    }
  }
  //Now mixed tensor1 and tensor2 levels
  for(std::vector<std::map<std::string, int> >::const_iterator cvi = vars.begin(); cvi != vars.end(); ++cvi){
    std::map<std::string, int> interleavedVars = *cvi;
    int * domains = new int[6 * interleavedVars.size()];
    vari = 0;
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      for(int j=vari * 6; j < 6 * (vari + 1); ++j)
        domains[j] = cmi->second;
      vari++;
    }
    //Now actually create the fdd levels
    // lock mutex
    int base = fdd_extdomain(domains, 6 * interleavedVars.size());
    // release mutex
    if (base < 0){
      *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
      *waliErr << "    Aborting." << endl;
      assert (false);
    }
    delete [] domains;
    // Assign fdd levels to the variables.
    vari = 0;
    for(std::map<std::string, int>::const_iterator cmi = interleavedVars.begin(); cmi != interleavedVars.end(); ++cmi){
      bddinfo_t varInfo = (*this)[cmi->first];
      int j = 6 * vari;
      varInfo->tensor1Lhs = base + j++;
      varInfo->tensor1Rhs = base + j++;
      varInfo->tensor1Extra = base + j++;
      varInfo->tensor2Lhs = base + j++;
      varInfo->tensor2Rhs = base + j++;
      varInfo->tensor2Extra = base + j++;
      vari++;  
    }
  }
#else
#error "Unknown bdd level arrangement macro"
#endif

  // Also update the reverse vocabulary for printing.
  for(std::map<const std::string, bddinfo_t>::const_iterator ci = this->begin(); ci != this->end(); ++ci){
    bddinfo_t varInfo = ci->second;
    idx2Name[varInfo->baseLhs] = ci->first;
    idx2Name[varInfo->baseRhs] = ci->first + "'";
    idx2Name[varInfo->baseExtra] = ci->first + "''";
    idx2Name[varInfo->tensor1Lhs] = ci->first + "_t1";
    idx2Name[varInfo->tensor1Rhs] = ci->first + "_t1'";
    idx2Name[varInfo->tensor1Extra] = ci->first + "_t1''";
    idx2Name[varInfo->tensor2Lhs] = ci->first + "_t2";
    idx2Name[varInfo->tensor2Rhs] = ci->first + "_t2'";
    idx2Name[varInfo->tensor2Extra] = ci->first + "_t2''";
  } 

#if (NWA_DETENSOR == 1)
  setupLevelArray();
#endif
}

void BddContext::setIntVars(const std::vector<std::map<std::string, int> >& vars)
{
  createIntVars(vars);
  setupCachedBdds();
}

void BddContext::setupCachedBdds()
{
  int vari;

  // Update bddPairs
  // We will first create arrays for each of columns
  int * baseLhs = new int[this->size()];
  int * baseRhs = new int[this->size()];
  int * baseExtra = new int[this->size()];
  int * tensor1Lhs = new int[this->size()];
  int * tensor1Rhs = new int[this->size()];  
  int * tensor1Extra = new int[this->size()];
  int * tensor2Lhs = new int[this->size()];
  int * tensor2Rhs = new int[this->size()];
  int * tensor2Extra = new int[this->size()];
  vari = 0;
  for(std::map<const std::string, bddinfo_t>::const_iterator ci = this->begin(); ci != this->end(); ++ci){
    bddinfo_t varInfo = ci->second; 
    baseLhs[vari] = varInfo->baseLhs;
    baseRhs[vari] = varInfo->baseRhs;
    baseExtra[vari] = varInfo->baseExtra;
    tensor1Lhs[vari] = varInfo->tensor1Lhs;
    tensor1Rhs[vari] = varInfo->tensor1Rhs;
    tensor1Extra[vari] = varInfo->tensor1Extra;
    tensor2Lhs[vari] = varInfo->tensor2Lhs;
    tensor2Rhs[vari] = varInfo->tensor2Rhs;
    tensor2Extra[vari] = varInfo->tensor2Extra;
    vari++; 
  }

  //DEBUGGING
#if 0
  cout << "baseLhs: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << baseLhs[i];
  cout << endl;
  cout << "baseRhs: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << baseRhs[i];
  cout << endl;
  cout << "baseExtra: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << baseExtra[i];
  cout << endl;
  cout << "tensor1Lhs: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << tensor1Lhs[i];
  cout << endl;
  cout << "tensor1Rhs: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << tensor1Rhs[i];
  cout << endl;
  cout << "tensor1Extra: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << tensor1Extra[i];
  cout << endl;
  cout << "tensor2Lhs: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << tensor2Lhs[i];
  cout << endl;
  cout << "tensor2Rhs: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << tensor2Rhs[i];
  cout << endl;
  cout << "tensor2Extra: ";
  for(unsigned i =0; i < this->size(); ++i)
    cout << " " << tensor2Extra[i];
  cout << endl;
#endif
  //END DEBUGGING

  fdd_setpairs(baseSwap.get(), baseLhs, baseRhs, this->size());
  fdd_setpairs(baseSwap.get(), baseRhs, baseLhs, this->size());
  fdd_setpairs(tensor1Swap.get(), tensor1Lhs, tensor1Rhs, this->size());
  fdd_setpairs(tensor1Swap.get(), tensor1Rhs, tensor1Lhs, this->size());
  fdd_setpairs(baseRightShift.get(), baseLhs, baseRhs, this->size());
  fdd_setpairs(baseRightShift.get(), baseRhs, baseExtra, this->size());
  fdd_setpairs(tensorRightShift.get(), tensor1Lhs, tensor1Rhs, this->size());
  fdd_setpairs(tensorRightShift.get(), tensor2Lhs, tensor2Rhs, this->size());
  fdd_setpairs(tensorRightShift.get(), tensor1Rhs, tensor1Extra, this->size());
  fdd_setpairs(tensorRightShift.get(), tensor2Rhs, tensor2Extra, this->size());
  fdd_setpairs(baseRestore.get(), baseExtra, baseRhs, this->size());
  fdd_setpairs(tensorRestore.get(), tensor1Extra, tensor1Rhs, this->size());
  fdd_setpairs(tensorRestore.get(), tensor2Extra, tensor2Rhs, this->size());
  fdd_setpairs(move2Tensor1.get(), baseLhs, tensor1Lhs, this->size());
  fdd_setpairs(move2Tensor1.get(), baseRhs, tensor1Rhs, this->size());
  fdd_setpairs(move2Tensor2.get(), baseLhs, tensor2Lhs, this->size());
  fdd_setpairs(move2Tensor2.get(), baseRhs, tensor2Rhs, this->size());
  fdd_setpairs(move2Base.get(), tensor1Lhs, baseLhs, this->size());
  fdd_setpairs(move2Base.get(), tensor2Rhs, baseRhs, this->size());
  fdd_setpairs(move2BaseTwisted.get(), tensor1Lhs, baseLhs, this->size());
  fdd_setpairs(move2BaseTwisted.get(), tensor2Lhs, baseRhs, this->size());

  // Update static bdds
  baseSecBddContextSet = fdd_makeset(baseRhs, this->size());
  tensorSecBddContextSet = fdd_makeset(tensor1Rhs, this->size());
  tensorSecBddContextSet &= fdd_makeset(tensor2Rhs, this->size());
  commonBddContextSet23 = fdd_makeset(tensor1Rhs, this->size());
  commonBddContextSet23 &= fdd_makeset(tensor2Lhs, this->size());
  commonBddContextSet13 = fdd_makeset(tensor1Lhs, this->size());
  commonBddContextSet13 &= fdd_makeset(tensor2Lhs, this->size());
  assert(this->size() == 0 || (baseSecBddContextSet != bddfalse && tensorSecBddContextSet != bddfalse
        && tensorSecBddContextSet != bddfalse && commonBddContextSet23 != bddfalse && commonBddContextSet13 != bddfalse));
#if (DETENSOR_TOGETHER == 1)
  // Somehow make this efficient
  for(std::map<const std::string, bddinfo_t>::const_iterator ci = this->begin(); ci != this->end(); ++ci){
    bddinfo_t varInfo = ci->second;
    commonBddContextId23 = commonBddContextId23 &
      fdd_equals(varInfo->tensor1Rhs, varInfo->tensor2Lhs);
    commonBddContextId13 = commonBddContextId13 & 
      fdd_equals(varInfo->tensor1Lhs, varInfo->tensor2Lhs);
  }
#endif

  // Create cached BinRel objects
  // Somehow make this efficient
  bdd baseId = bddtrue;
  bdd tensorId = bddtrue;
  for(BddContextIter varIter = this->begin(); varIter != this->end();  ++varIter){
    bddinfo_t varInfo = (*varIter).second;
    baseId = baseId & fdd_equals(varInfo->baseLhs, varInfo->baseRhs);
    tensorId = tensorId & (fdd_equals(varInfo->tensor1Lhs, varInfo->tensor1Rhs) & 
        fdd_equals(varInfo->tensor2Lhs, varInfo->tensor2Rhs));
  }
  cachedBaseOne = new BinRel(this, baseId, false);
  cachedBaseZero = new BinRel(this, bddfalse, false);
  cachedBaseTop = new BinRel(this, bddtrue, false);
  cachedTensorOne = new BinRel(this, tensorId, true);
  cachedTensorZero = new BinRel(this, bddfalse, true);
  cachedTensorTop = new BinRel(this, bddtrue, true);

  delete [] baseLhs;
  delete [] baseRhs;
  delete [] baseExtra;
  delete [] tensor1Lhs;
  delete [] tensor1Rhs;
  delete [] tensor1Extra;
  delete [] tensor2Lhs;
  delete [] tensor2Rhs;
  delete [] tensor2Extra;
}

binrel_t BddContext::getBaseOne() const
{
  return cachedBaseOne; 
}

binrel_t BddContext::getBaseZero() const
{
  return cachedBaseZero;
}

binrel_t BddContext::getTensorOne() const
{
  return cachedTensorOne;
}

binrel_t BddContext::getTensorZero() const
{
  return cachedTensorZero;
}

binrel_t BddContext::getBaseTop() const
{
  return cachedBaseTop;
}

binrel_t BddContext::getTensorTop() const
{
  return cachedTensorTop;
}

void BddContext::addIntVar(std::string name, unsigned size)
{
  //create and insert a new BddInfo into the vocabulary
  if(size < 2){
    *waliErr << "I haven't tested the library for int size less than 2";
    assert(false);
  }
  bddinfo_t varInfo = new BddInfo;
#if (NWA_DETENSOR == 1)
  // Only boolean variables are supported under NWA_DETENSOR
  assert(size == 2);
#endif
  varInfo->maxVal = size;
  (*this)[name] = varInfo;

  //Create space in buddy for this variable
  //We will create indices such that we get a default variable ordering where
  //baseLhs, baseRhs, baseExtra are mixed.
  //tensor1Lhs, tensor1Rhs, tensor1Extra are mixed.
  //tensor2Lhs, tensor2Rhs, tensor2Extra are mixed.

  //lock mutex
  int base;
  int domains[3] = {(int)varInfo->maxVal, (int)varInfo->maxVal, (int)varInfo->maxVal};

  //Create fdds for base
  base = fdd_extdomain(domains,3);
  if (base < 0){
    *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
    *waliErr << "    Aborting." << endl;
    assert (false);
  }
  varInfo->baseLhs = base;
  varInfo->baseRhs = base + 1;
  varInfo->baseExtra = base + 2;

  //create fdds for tensor1
  base = fdd_extdomain(domains,3);
  if (base < 0){
    *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
    *waliErr << "    Aborting." << endl;
    assert (false);
  }
  varInfo->tensor1Lhs = base;
  varInfo->tensor1Rhs = base + 1;
  varInfo->tensor1Extra = base + 2;

  //create fdds for tensor2
  base = fdd_extdomain(domains,3);
  if (base < 0){
    *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
    *waliErr << "    Aborting." << endl;
    assert (false);
  }
  varInfo->tensor2Lhs = base;
  varInfo->tensor2Rhs = base + 1;
  varInfo->tensor2Extra = base + 2;
  //release mutex

  //We will now update all the cached bdds and bddpairs 
  //update bddPairs
  fdd_setpair(baseSwap.get(), varInfo->baseLhs, varInfo->baseRhs);
  fdd_setpair(baseSwap.get(), varInfo->baseRhs, varInfo->baseLhs);
  fdd_setpair(tensor1Swap.get(), varInfo->tensor1Lhs, varInfo->tensor1Rhs);
  fdd_setpair(tensor1Swap.get(), varInfo->tensor1Rhs, varInfo->tensor1Lhs);
  fdd_setpair(baseRightShift.get(), varInfo->baseLhs, varInfo->baseRhs);
  fdd_setpair(baseRightShift.get(), varInfo->baseRhs, varInfo->baseExtra);
  fdd_setpair(tensorRightShift.get(), varInfo->tensor1Lhs, varInfo->tensor1Rhs);
  fdd_setpair(tensorRightShift.get(), varInfo->tensor2Lhs, varInfo->tensor2Rhs);
  fdd_setpair(tensorRightShift.get(), varInfo->tensor1Rhs, varInfo->tensor1Extra);
  fdd_setpair(tensorRightShift.get(), varInfo->tensor2Rhs, varInfo->tensor2Extra);
  fdd_setpair(baseRestore.get(), varInfo->baseExtra, varInfo->baseRhs);
  fdd_setpair(tensorRestore.get(), varInfo->tensor1Extra, varInfo->tensor1Rhs);
  fdd_setpair(tensorRestore.get(), varInfo->tensor2Extra, varInfo->tensor2Rhs);
  fdd_setpair(move2Tensor1.get(), varInfo->baseLhs, varInfo->tensor1Lhs);
  fdd_setpair(move2Tensor1.get(), varInfo->baseRhs, varInfo->tensor1Rhs);
  fdd_setpair(move2Tensor2.get(), varInfo->baseLhs, varInfo->tensor2Lhs);
  fdd_setpair(move2Tensor2.get(), varInfo->baseRhs, varInfo->tensor2Rhs);
  fdd_setpair(move2Base.get(), varInfo->tensor1Lhs, varInfo->baseLhs);
  fdd_setpair(move2Base.get(), varInfo->tensor2Rhs, varInfo->baseRhs);
  fdd_setpair(move2BaseTwisted.get(), varInfo->tensor1Lhs, varInfo->baseLhs);
  fdd_setpair(move2BaseTwisted.get(), varInfo->tensor2Lhs, varInfo->baseRhs);
  //update static bdds
  baseSecBddContextSet = baseSecBddContextSet & fdd_ithset(varInfo->baseRhs);
  tensorSecBddContextSet = tensorSecBddContextSet & fdd_ithset(varInfo->tensor1Rhs);
  tensorSecBddContextSet = tensorSecBddContextSet & fdd_ithset(varInfo->tensor2Rhs);
  commonBddContextSet23 = commonBddContextSet23 & fdd_ithset(varInfo->tensor1Rhs);
  commonBddContextSet23 = commonBddContextSet23 & fdd_ithset(varInfo->tensor2Lhs);
  commonBddContextId23 = commonBddContextId23 &
    fdd_equals(varInfo->tensor1Rhs, varInfo->tensor2Lhs);
  commonBddContextSet13 = commonBddContextSet13 & fdd_ithset(varInfo->tensor1Lhs);
  commonBddContextSet13 = commonBddContextSet13 & fdd_ithset(varInfo->tensor2Lhs);
  commonBddContextId13 = commonBddContextId13 & 
    fdd_equals(varInfo->tensor1Lhs, varInfo->tensor2Lhs);

  //update cached BinRel objects
  populateCache();
  //update RevBddContext for pretty printing through buddy.
  //lock mutex
  idx2Name[varInfo->baseLhs] = name;
  idx2Name[varInfo->baseRhs] = name + "'";
  idx2Name[varInfo->baseExtra] = name + "''";
  idx2Name[varInfo->tensor1Lhs] = name + "_t1";
  idx2Name[varInfo->tensor1Rhs] = name + "_t1'";
  idx2Name[varInfo->tensor1Extra] = name + "_t1''";
  idx2Name[varInfo->tensor2Lhs] = name + "_t2";
  idx2Name[varInfo->tensor2Rhs] = name + "_t2'";
  idx2Name[varInfo->tensor2Extra] = name + "_t2''";
  //release mutex
}

void BddContext::populateCache()
{
  bdd baseId = bddtrue;
  bdd tensorId = bddtrue;
  for(
      BddContextIter varIter = this->begin();
      varIter != this->end();
      ++varIter
     ){
    bddinfo_t varInfo = (*varIter).second;
    baseId = baseId &
      fdd_equals(varInfo->baseLhs, varInfo->baseRhs);
    tensorId = tensorId &
      (fdd_equals(varInfo->tensor1Lhs, varInfo->tensor1Rhs) &
       fdd_equals(varInfo->tensor2Lhs, varInfo->tensor2Rhs));
  }
  cachedBaseOne = new BinRel(this, baseId, false);
  cachedBaseZero = new BinRel(this, bddfalse, false);
  cachedBaseTop = new BinRel(this, bddtrue, false);
  cachedTensorOne = new BinRel(this, tensorId, true);
  cachedTensorZero = new BinRel(this, bddfalse, true);
  cachedTensorTop = new BinRel(this, bddtrue, true);

#if (NWA_DETENSOR == 1)
  setupLevelArray();
#endif
}

// ////////////////////////////
// Static
void BinRel::reset()
{
}

// ////////////////////////////
// Friends
namespace wali{
  namespace domains{
    namespace binrel{
      binrel_t operator*(binrel_t a, binrel_t b)
      {
        return a->Compose(b);
      }

      binrel_t operator|(binrel_t a, binrel_t b)
      {
        return a->Union(b);
      }

      binrel_t operator&(binrel_t a, binrel_t b)
      {
        return a->Intersect(b);
      }
    }
  }
}
// ////////////////////////////

// ////////////////////////////
// Members and Con/Destructors
BinRel::BinRel(const BinRel& that) : 
  wali::SemElemTensor(that),
  con(that.con),
  rel(that.rel),
  isTensored(that.isTensored)
{}

BinRel::BinRel(BddContext const * c, bdd b, bool it) : 
  con(c),
  rel(b), 
  isTensored(it) 
{}
BinRel::~BinRel() {}
// ////////////////////////////

binrel_t BinRel::Compose( binrel_t that ) const
{
#ifdef BINREL_STATS
  con->numCompose++;
#endif
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Composing incompatible relations" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con,bddfalse,isTensored);
  }
#endif
  if (this->isZero() || that->isZero())
    return static_cast<BinRel*>(zero().get_ptr());;
  if (this->isOne())
    return that;
  if (that->isOne())
    return new BinRel(*this);

  bdd c;
  if(!isTensored){
    bdd temp1 = bdd_replace(that->rel, con->baseRightShift.get());
    bdd temp2 = bdd_relprod(rel, temp1, con->baseSecBddContextSet);
    c = bdd_replace(temp2, con->baseRestore.get());
  }else{
    bdd temp1 = bdd_replace(that->rel, con->tensorRightShift.get());
    bdd temp2 = bdd_relprod(rel, temp1, con->tensorSecBddContextSet);
    c = bdd_replace(temp2, con->tensorRestore.get());
  }

  binrel_t ret = new BinRel(con,c,isTensored);
  // Keep zero/one unique.
  if(ret->isZero())
    return static_cast<BinRel*>(ret->zero().get_ptr());
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  return ret;
}

binrel_t BinRel::Union( binrel_t that ) const
{
#ifdef BINREL_STATS
  con->numUnion++;
#endif
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Unioning incompatible relations" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con,bddtrue,isTensored);
  }
#endif
  if (this->isZero())
    return that;
  if (that->isZero())
    return new BinRel(*this);

  // Keep zero/one unique
  binrel_t ret = new BinRel(con,rel | that->rel, isTensored);
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  //can't be zero.
  return ret;
}

binrel_t BinRel::Intersect( binrel_t that ) const
{
#ifdef BINREL_STATS
  con->numIntersect++;
#endif
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Intersecting incompatible relations" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con,bddfalse,isTensored);
  }
#endif
  if(this->isZero() || that->isZero())
    return static_cast<BinRel*>(this->zero().get_ptr());

  // Keep zero/one unique
  binrel_t ret = new BinRel(con, rel & that->rel,isTensored);
  if(ret->isZero())
    return static_cast<BinRel*>(ret->zero().get_ptr());
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  return ret;
}



bool BinRel::underApproximates(SemElem * se)
{
  binrel_t that = convert(se);
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    std::cerr << "con: " << con << "\n";
    std::cerr << "that->con: " << that->con << "\n";
    *waliErr << "[WARNING] " << "Compared (containment) incompatible relations" 
      << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return false;
  }
#endif
  return details::bddImplies(rel, that->rel);
}


bool BinRel::Equal( binrel_t that) const
{
#ifdef BINREL_STATS
  con->numEqual++;
#endif
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    std::cerr << "con: " << con << "\n";
    std::cerr << "that->con: " << that->con << "\n";
    *waliErr << "[WARNING] " << "Compared (Equality) incompatible relations" 
      << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return false;
  }
#endif
  return rel == that->rel;
}

binrel_t BinRel::Transpose() const
{
#ifdef BINREL_STATS
  con->numTranspose++;
#endif
#ifndef BINREL_HASTY
  if(isTensored){
    *waliErr << "[WARNING] " << "Attempted to transpose tensored weight."
      << endl << "Not supported" << endl;
    print(*waliErr) << endl;
    assert(false);
    return new BinRel(con, bddfalse, true);
  }
#endif
  if(this->isZero())
    return static_cast<BinRel*>(zero().get_ptr());
  if(this->isOne())
    return static_cast<BinRel*>(one().get_ptr());

  bdd c = bdd_replace(rel, con->baseSwap.get());
  return new BinRel(con, c, isTensored);
}

binrel_t BinRel::Kronecker(binrel_t that) const
{
#ifdef BINREL_STATS
  con->numKronecker++;
#endif
#ifndef BINREL_HASTY
  if(isTensored || that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Attempted to tensor two tensored weights OR attempted to tensor uncompatible relations."
      << endl << "Not supported" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con, bddfalse, true);
  }
#endif
  if(rel == bddfalse || that->rel == bddfalse)
    return con->cachedTensorZero;
#if (NWA_DETENSOR == 1)
  bdd c = tensorViaDetensor(that->Transpose()->rel); //nwa_detensor.cpp
#else
  bdd rel1 = bdd_replace(rel, con->move2Tensor1.get());
  bdd rel2 = bdd_replace(that->rel, con->move2Tensor2.get());
  bdd c = rel1 & rel2;
#endif
  binrel_t ret = new BinRel(con, c,true);
  if(ret->isZero())
    return static_cast<BinRel*>(ret->zero().get_ptr());
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  return ret;
}

binrel_t BinRel::Eq23Project() const
{
#ifndef BINREL_HASTY
  if(!isTensored){
    *waliErr << "[WARNING] " << "Attempted to detensor untensored weight."
      << endl;
    print(*waliErr) << endl;
    assert(false);
    return new BinRel(con,bddfalse, false);
  }
#endif
#if (DETENSOR_TOGETHER == 1)
  bdd rel1 = rel & con->commonBddContextId23; 
  bdd rel2 = bdd_exist(rel1, con->commonBddContextSet23);
  bdd c = bdd_replace(rel2, con->move2Base.get());
#else
  bdd rel1 = rel;
  for(std::map<const std::string, bddinfo_t>::const_iterator citer = con->begin(); citer != con->end(); ++citer){
    bddinfo_t varInfo = (*citer).second;
    bdd id = fdd_equals(varInfo->tensor1Rhs, varInfo->tensor2Lhs);
    rel1 = rel1 & id;
    rel1 = bdd_exist(rel1, fdd_ithset(varInfo->tensor1Rhs) & fdd_ithset(varInfo->tensor2Lhs));
  }
  bdd c = bdd_replace(rel1, con->move2Base.get());
#endif
  binrel_t ret = new BinRel(con,c,false);
  if(ret->isZero())
    return static_cast<BinRel*>(ret->zero().get_ptr());
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  return ret;
}

binrel_t BinRel::Eq13Project() const
{
#ifndef BINREL_HASTY
  if(!isTensored){
    *waliErr << "[WARNING] " << "Attempted to detensor untensored weight."
      << endl;
    print(*waliErr) << endl;
    assert(false);
    return new BinRel(con,bddfalse, false);
  }
#endif
#if (DETENSOR_TOGETHER == 1)
  bdd rel1 = rel & con->commonBddContextId13; 
  bdd rel2 = bdd_exist(rel1, con->commonBddContextSet13);
  bdd c = bdd_replace(rel2, con->move2BaseTwisted.get());
#else
  bdd rel1 = rel;
  for(std::map<const std::string, bddinfo_t>::const_iterator citer = con->begin(); citer != con->end(); ++citer){
    bddinfo_t varInfo = (*citer).second;
    bdd id = fdd_equals(varInfo->tensor1Rhs, varInfo->tensor2Rhs);
    rel1 = rel1 & id;
    rel1 = bdd_exist(rel1, fdd_ithset(varInfo->tensor1Rhs) & fdd_ithset(varInfo->tensor2Rhs));
  }
  bdd c = bdd_replace(rel1, con->move2BaseTwisted.get());
#endif
  binrel_t ret = new BinRel(con,c,false);
  if(ret->isZero())
    return static_cast<BinRel*>(ret->zero().get_ptr());
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  return ret;
}



// ////////////////////////////
// SemElem Interface functions

wali::sem_elem_t BinRel::star()
{
  StarCacheKey k(getBdd(), isTensored);
  if (star_cache.find(k) == star_cache.end()) {
    sem_elem_t w = combine(one().get_ptr());
    sem_elem_t wn = w->extend(w);
    while(!w->equal(wn)) {
      w = wn;
      wn = wn->extend(wn);
    }
    star_cache[k] = wn;
  }

  return star_cache[k];
}


wali::sem_elem_t BinRel::combine(wali::SemElem* se) 
{
  binrel_t that( convert(se) );
  return Union(that);
}

wali::sem_elem_t BinRel::extend(wali::SemElem* se) 
{
  binrel_t that( convert(se) );
  return Compose(that);
}

bool BinRel::equal(wali::SemElem* se) const 
{
  binrel_t that( convert(se) );
  return Equal(that);
}

bool BinRel::containerLessThan(wali::SemElem const * se) const
{
  BinRel const * other = dynamic_cast<BinRel const *>(se);
  return this->getBdd().id() < other->getBdd().id();
}


wali::sem_elem_t BinRel::one() const
{
  if (!isTensored)
    return con->cachedBaseOne;
  else
    return con->cachedTensorOne;
}

wali::sem_elem_t BinRel::zero() const
{
  if(!isTensored)
    return con->cachedBaseZero;
  else
    return con->cachedTensorZero;
}

std::ostream& BinRel::print( std::ostream& o ) const 
{
  if(!isTensored)
    o << "Base relation: ";
  else
    o << "Tensored relation: ";
  
  o << fddset << rel;
  return o;
}

wali::sem_elem_tensor_t BinRel::transpose() 
{
  return Transpose();
}

wali::sem_elem_tensor_t BinRel::tensor(wali::SemElemTensor* se)
{
  binrel_t that( convert(se) );
  return Kronecker(that);
}

wali::sem_elem_tensor_t BinRel::detensor()
{
#ifdef BINREL_STATS
  con->numDetensor++;
#endif
  return Eq23Project();
}

wali::sem_elem_tensor_t BinRel::detensorTranspose()
{
#ifdef BINREL_STATS
  con->numDetensorTranspose++;
#endif
#if (NWA_DETENSOR == 1)
  assert(isTensored);
  //bdd c = detensorViaNwa();
  binrel_t ret = new BinRel(con, detensorViaNwa());
  if(ret->isZero())
    return static_cast<BinRel*>(ret->zero().get_ptr());
  if(ret->isOne())
    return static_cast<BinRel*>(ret->one().get_ptr());
  return ret;
#else
  return Eq13Project();
#endif
}

#ifdef BINREL_STATS
std::ostream& BddContext::printStats( std::ostream& o) const
{
  o << "BinRel Statistics:" <<endl;
  o << "#Compose: " << numCompose << endl; 
  o << "#Union: " << numUnion << endl; 
  o << "#Intersect: " << numIntersect << endl; 
  o << "#Equal: " << numEqual << endl; 
  o << "#Kronecker: " << numKronecker << endl;
#if (NWA_DETENSOR == 1)
  o << "#Reverse: " << numReverse << endl;
#endif
  o << "#Transpose: " << numTranspose << endl;
  o << "#Eq23Project: " << numDetensor << endl;
  o << "#Eq13Project: " << numDetensorTranspose << endl;
  return o;
}

void BddContext::resetStats()
{
  numCompose = 0;
  numUnion = 0;
  numIntersect = 0;
  numEqual = 0;
  numKronecker = 0;
  numReverse = 0;
  numTranspose = 0;
  numDetensor = 0;
  numDetensorTranspose = 0;
}
#endif //BINREL_STATS

///////////////////////////////

namespace wali {
  namespace domains {
    namespace binrel {

      bdd
      quantifyOutOtherVariables(BddContext const & voc,
                                std::vector<std::string> const & keep_these,
                                bdd b)
      {
        std::vector<int> keep_these_fdds;
        for (std::vector<std::string>::const_iterator keep_this = keep_these.begin();
             keep_this != keep_these.end(); ++keep_this)
        {
          BddContext::const_iterator var_info = voc.find(*keep_this);
          assert(var_info != voc.end());
          
          keep_these_fdds.push_back(var_info->second->baseLhs);
          keep_these_fdds.push_back(var_info->second->baseRhs);
        }

        bdd vars_to_remove = bddtrue;

        for (int fdd_num=0; fdd_num<fdd_domainnum(); ++fdd_num)
        {
          if (std::find(keep_these_fdds.begin(),
                        keep_these_fdds.end(),
                        fdd_num)
              == keep_these_fdds.end())
          {
            vars_to_remove &= fdd_ithset(fdd_num);
          }
        }

        return bdd_exist(b, vars_to_remove);
      }
      
      
      std::vector<Assignment>
      getAllAssignments(VectorVocabulary const & voc);
      
      std::vector<Assignment>
      getAllAssignments(BddContext const & voc)
      {
        VectorVocabulary voc_vec(voc.begin(), voc.end());
        return getAllAssignments(voc_vec);
      }

      std::vector<Assignment>
      getAllAssignments(VectorVocabulary const & voc)
      {
        // Make a copy so we can index by number instead of by name. (A
        // vector is just a map from int to something.)
        std::vector<std::pair<std::string, bddinfo_t> > voc_vector(voc.begin(), voc.end());
        bddinfo_t bddinfo = new BddInfo();
        bddinfo->maxVal = 17;
        voc_vector.push_back(VectorVocabulary::value_type("__dummy__", bddinfo));
        
        std::vector<Assignment> result;

        // The current assignment. Essentially [0..max)^n, except the limit
        // may change depending on coordinate and we add one more to the end
        // as sort of an elephant. When we overflow into
        // assignment_vec[voc.size()], then we're done.
        std::vector<unsigned int> assignment_vec(voc.size()+1, 0);
          
        while(assignment_vec.at(voc.size()) == 0) {
          // Create the current assignment
          Assignment assignment;
          for (size_t varno=0; varno<voc.size(); ++varno) {
            if (!boost::starts_with(voc_vector.at(varno).first,
                                    "zDUMMY")) {
              assignment[voc_vector.at(varno).first] = assignment_vec.at(varno);
            }
          }
          if (std::find(result.begin(), result.end(), assignment) == result.end()) {
            result.push_back(assignment);
          }
        
          // Increment the assignment vector for next iteration.
          assert(assignment_vec.at(assignment_vec.size()-1) == 0);
          for (size_t placeno=0; true; ++placeno) {
            assignment_vec.at(placeno) += 1;
            // Do we overflow?
            if (assignment_vec.at(placeno) == voc_vector.at(placeno).second->maxVal) {
              // Yes: reset to 0 and keep going
              assignment_vec.at(placeno) = 0;
            }
            else {
              // No overflow: so we're done adding
              break;
            }
          }
        }
          
        return result;
      }

      template<typename Mapping>
      typename Mapping::mapped_type
      safe_get(Mapping const & m, typename Mapping::key_type const & k)
      {
        typename Mapping::const_iterator it = m.find(k);
        if (it == m.end()) {
          assert(false);
        }
        return it->second;
      }

      enum VocabularyId { PRE_VOCABULARY, POST_VOCABULARY };

      bdd toBdd(Assignment const & assignment, BddContext const & voc, VocabularyId vid)
      {
        bdd result = bddtrue;
        for (Assignment::const_iterator pair = assignment.begin();
             pair != assignment.end(); ++pair)
          
        {
          bddinfo_t varinfo = safe_get(voc, pair->first);
          int val = pair->second;

          bdd b;
          if (vid == PRE_VOCABULARY) {
            b = fdd_ithvar(varinfo->baseLhs, val);
          }
          else {
            b = fdd_ithvar(varinfo->baseRhs, val);
          }
          
          result &= b;
        }
        return result;
      }


      namespace details {
        bool is_feasible(Assignment const & pre_assgn,
                         Assignment const & post_assgn,
                         BddContext const & voc,
                         bdd b)
        {
          bdd pre_bdd = toBdd(pre_assgn, voc, PRE_VOCABULARY);
          bdd post_bdd = toBdd(post_assgn, voc, POST_VOCABULARY);

          bdd everything = pre_bdd & post_bdd & b;

          return (everything != bddfalse);
        }

        const int width = 150;
        const int pre_x_center = 15;
        const int post_x_center = 50;
        
        const int radius = 2;

        const int v_margin = 25;
        const int v_separation = 25;


        void write_line(int pre_no, int post_no, std::ostream & os, int base_y)
        {
          int x1 = pre_x_center;
          int y1 = v_margin + pre_no * v_separation + base_y;

          int x2 = post_x_center;
          int y2 = v_margin + post_no * v_separation + base_y;

          os << "    -draw \"line "
             << x1 << "," << y1 << " "
             << x2 << "," << y2 << "\"    \\\n";
        }

        std::string toStringNums(Assignment const & assgn)
        {
          std::stringstream ss;
          Assignment::const_iterator iter = assgn.begin();

          ss << "(" << iter->second;
          ++iter;

          for(; iter != assgn.end(); ++iter) {
            ss << ", " << iter->second;
          }

          ss << ")";
          return ss.str();
        }

        std::string toString(Assignment const & assgn)
        {
          std::stringstream ss;
          Assignment::const_iterator iter = assgn.begin();

          ss << "(" << iter->first << "=" << iter->second;
          ++iter;

          for(; iter != assgn.end(); ++iter) {
            ss << ", " << iter->first << "=" << iter->second;
          }

          ss << ")";
          return ss.str();
        }
        
        std::string toStringNames(Assignment const & assgn)
        {
          std::stringstream ss;
          Assignment::const_iterator iter = assgn.begin();

          ss << "(" << iter->first;
          ++iter;

          for(; iter != assgn.end(); ++iter) {
            ss << ", " << iter->first;
          }

          ss << ")";
          return ss.str();
        }

        void write_dot(int dot_no, Assignment const & assgn, std::ostream & os, int base_y)
        {
          int x = pre_x_center;
          int y = v_margin + dot_no * v_separation + base_y;

          os << "    -draw \"circle "
             << x << "," << y << " "
             << (x-radius) << "," << (y-radius) << "\"    ";

          x = post_x_center;          

          os << "    -draw \"circle "
             << x << "," << y << " "
             << (x-radius) << "," << (y-radius) << "\"    ";

          x = post_x_center + pre_x_center;
          os << "    -draw \"text "
             << x << "," << y << " "
             << "\'" << toStringNums(assgn) << "\'\"    \\\n";
        }

      
        std::vector<std::pair<VectorVocabulary, bdd> >
        partition(BddContext const & vars, bdd b);

      }
      
      void
      printImagemagickInstructions(bdd b, BddContext const & voc, std::ostream & os, std::string const & for_file,
                                   boost::function<bool (VectorVocabulary const &)> include_component)
      {
        std::vector<std::pair<VectorVocabulary, bdd> > independent_components = details::partition(voc, b);
        std::vector<std::vector<Assignment> > possible_assignments_by_component;

        int total_height = 0;
        for (size_t comp_no=0; comp_no<independent_components.size(); ++comp_no) {
          VectorVocabulary const & comp_voc = independent_components.at(comp_no).first;
          std::vector<Assignment> possibleAssignments = getAllAssignments(comp_voc);
          possible_assignments_by_component.push_back(possibleAssignments);
          
          assert(comp_voc.size() > 0);
          if(boost::starts_with(comp_voc.begin()->first, "DUMMY")) {
            assert(comp_voc.size() == 1);
            continue;
          }

          if(include_component && !include_component(comp_voc)) {
            continue;
          }
            
          total_height += 2*details::v_margin;
          total_height += possibleAssignments.size() * details::v_separation;
        }

        os << "convert -size " << details::width << "x" << total_height
           << " xc:wheat    \\\n"
           << "    -font Times-Roman  \\\n";

        int current_base_y = 0;
        
        for (size_t comp_no=0; comp_no<independent_components.size(); ++comp_no) {
          VectorVocabulary const & comp_voc = independent_components.at(comp_no).first;
          if(boost::starts_with(comp_voc.begin()->first, "DUMMY")) {
            assert(comp_voc.size() == 1);
            continue;
          }

          if(include_component && !include_component(comp_voc)) {
            continue;
          }
          
          bdd comp_bdd = independent_components.at(comp_no).second;
          std::vector<Assignment> const & possibleAssignments = possible_assignments_by_component.at(comp_no);

          os << "    -draw \"text 0," << (current_base_y + 12) << " \'" << details::toStringNames(possibleAssignments[0]) << "\'\"    \\\n"
             << "    -fill red    \\\n";

          // Draw the lines
          for (size_t pre_no=0; pre_no<possibleAssignments.size(); ++pre_no) {
            for (size_t post_no=0; post_no<possibleAssignments.size(); ++post_no) {
              if (details::is_feasible(possibleAssignments.at(pre_no),
                                       possibleAssignments.at(post_no),
                                       voc,
                                       comp_bdd))
              {
                details::write_line(pre_no, post_no, os, current_base_y);
              }
            }
          }
          
          os << "    -fill black    \\\n";

          // Draw the dots
          for (size_t dot_no=0; dot_no < possibleAssignments.size(); ++dot_no) {
            details::write_dot(dot_no, possibleAssignments.at(dot_no), os, current_base_y);
          }

          current_base_y += 2*details::v_margin + possibleAssignments.size()*details::v_separation;          
        }

        os << "    " << for_file << "\n";        
      }



      namespace details {

        ///////////////
        
        ///////////////
        
        bdd to_bdd_varset(VectorVocabulary const & vars)
        {
          bdd result = bddtrue;
          for(VectorVocabulary::const_iterator var = vars.begin(); var != vars.end(); ++var) {
            int fdd_no_left = var->second->baseLhs;
            int fdd_no_right = var->second->baseRhs;

            result &= fdd_ithset(fdd_no_left) & fdd_ithset(fdd_no_right);
          }
          return result;
        }



        bool are_independent(VectorVocabulary const & vars_a, bdd & ra,
                             VectorVocabulary const & vars_b, bdd & rb,
                             bdd b)
        {
          ra = bdd_exist(b, to_bdd_varset(vars_b));
          rb = bdd_exist(b, to_bdd_varset(vars_a));

          return (ra & rb) == b;
        }


        bool
        first_less_than(std::pair<std::string, bddinfo_t> const & left,
                        std::pair<std::string, bddinfo_t> const & right)
        {
          return left.first < right.first;
        }

#define PRINT 0
        void partition(std::vector<std::pair<VectorVocabulary, bdd> > & cur_partition,
                       VectorVocabulary const & vars,
                       bdd b,
                       int starting_size)
        {
#if PRINT
          std::cout << "\n\npartition(starting_size=" << starting_size << "):\n";
#endif
          
          assert(vars.size() > 0);
          std::vector<std::pair<std::string, bddinfo_t> > voc_vec(vars.begin(), vars.end());
          
          for(size_t i=starting_size; i<vars.size(); ++i) {
            // for each combination C of size I from Vars.
            std::vector<std::pair<std::string, bddinfo_t> > combination(voc_vec.begin(), voc_vec.begin()+i);
            do
            {            
              VectorVocabulary voc_minus_combination;

              std::set_difference(voc_vec.begin(), voc_vec.end(), combination.begin(), combination.end(),
                                  std::inserter(voc_minus_combination, voc_minus_combination.begin()),
                                  first_less_than);

#if PRINT
              std::cout << "    Testing partition:\n";
               std::cout << "        [ ";
               for(VectorVocabulary::const_iterator vi=combination.begin(); vi!=combination.end(); ++vi) {
                 std::cout << vi->first << ", ";
               }
               std::cout << "]\n";
               std::cout << "        [ ";
               for(VectorVocabulary::const_iterator vi=voc_minus_combination.begin(); vi!=voc_minus_combination.end(); ++vi) {
                 std::cout << vi->first << ", ";
               }
               std::cout << "]\n";
#endif

              
              bdd ra, rb;
              if (are_independent(combination, ra, voc_minus_combination, rb, b))
              {
#if PRINT
                std::cout << "        are independent!\n";
#endif
                cur_partition.push_back(std::make_pair(combination, ra));
                partition(cur_partition, voc_minus_combination, rb, i);
                return;
              }
#if PRINT
              else {
                std::cout << "        are not independent!\n";
              }
#endif              
            }
            while(stdcomb::next_combination(voc_vec.begin(), voc_vec.end(),
                                            combination.begin(), combination.end()));
          }
          if (vars.size() > 0) {
            cur_partition.push_back(std::make_pair(vars, b));
          }
        }
        
        std::vector<std::pair<VectorVocabulary, bdd> >
        partition(BddContext const & vars, bdd b)
        {
          VectorVocabulary vars_vec(vars.begin(), vars.end());
          std::vector<std::pair<VectorVocabulary, bdd> > result;
          partition(result, vars_vec, b, 1);
          //result.push_back(std::make_pair(vars, b));
          return result;
        }


      }
      
    }
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

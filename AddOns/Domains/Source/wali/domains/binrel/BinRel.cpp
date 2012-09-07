/**
 * @author Nicholas Kidd
 * @author Prathmesh Prabhu
 * @version $Id: BinRel.cpp 557 2009-10-19 18:51:54Z kidd $
 */
 
#include "BinRel.hpp"
#include "buddy/fdd.h"
//#include "BuddyExt.hpp"

#include <iostream>
#include <sstream>

using namespace wali::domains::binrel;
using std::endl;
using wali::waliErr;

// ////////////////////////////
// Implementation of the initialization free function.
#ifdef BDDBIGMEM
// For large memory machines consider allocating ~1500 MB
// 25000000 ~~ 500 MB, change the multiple to get what you need.
#define FIVE_CENT_MB 25000000
#define BDDMEMSIZE (FIVE_CENT_MB*1)
#else
#define BDDMEMSIZE 10000000
#endif
#define CACHESIZE 100000
#define MAXMEMINC 100000

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
    }
  }
}


// ////////////////////////////
// Definitions of static members from BddContext/BinRel class

int BddContext::numBddContexts = 0;
#ifdef BINREL_STATS
StatCount BinRel::numCompose = 0;
StatCount BinRel::numUnion = 0;
StatCount BinRel::numIntersect = 0;
StatCount BinRel::numEqual = 0;
StatCount BinRel::numKronecker = 0;
StatCount BinRel::numTranspose = 0;
StatCount BinRel::numEq23Project = 0;
StatCount BinRel::numEq24Project = 0;
#endif
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
    cacheSize = (cacheSize==0)?CACHESIZE:cacheSize;
    if (0 == bdd_isrunning()){
      int rc = bdd_init(bddMemSize,cacheSize);
      if( rc < 0 ){
        *waliErr << "[ERROR] " << bdd_errstring(rc) << endl;
        assert( 0 );
      }
      // Default is 50,000 (1 Mb),memory is cheap, so use 100,000
      bdd_setmaxincrease(MAXMEMINC);
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
  baseRightShift = BddPairPtr(bdd_newpair());
  tensorRightShift = BddPairPtr(bdd_newpair());
  baseRestore = BddPairPtr(bdd_newpair());
  tensorRestore = BddPairPtr(bdd_newpair());
  move2Tensor1 = BddPairPtr(bdd_newpair());
  move2Tensor2 = BddPairPtr(bdd_newpair());
  move2Base = BddPairPtr(bdd_newpair());
  move2BaseTwisted = BddPairPtr(bdd_newpair());

  //initialize static bdds
  baseSecBddContextSet = bddtrue;
  tensorSecBddContextSet= bddtrue;
  commonBddContextSet23 = bddtrue;
  commonBddContextSet13 = bddtrue;
  commonBddContextId23 = bddtrue;
  commonBddContextId13 = bddtrue;

  cachedBaseOne = NULL;
  cachedBaseZero = NULL;
  cachedTensorOne = NULL;
  cachedTensorZero = NULL;
  populateCache();
}

//Don't copy count
BddContext::BddContext(const BddContext& other) :
  std::map< const std::string, bddinfo_t>(other),
  count(0),
  baseSwap(other.baseSwap),
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
  populateCache();
}

BddContext& BddContext::operator = (const BddContext& other)
{
  if(this!=&other){
    count=0;
    baseSwap=other.baseSwap;
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
  baseRightShift.reset();
  tensorRightShift.reset();
  baseRestore.reset();
  tensorRestore.reset();
  move2Tensor1.reset();
  move2Tensor2.reset();
  move2Base.reset();
  move2BaseTwisted.reset();
  
  baseSecBddContextSet = bddtrue;
  tensorSecBddContextSet = bddtrue;
  commonBddContextSet23 = bddtrue;
  commonBddContextId23 = bddtrue;
  commonBddContextSet13 = bddtrue;
  commonBddContextId13 = bddtrue;

  //Delete cached BinRel objects.
  cachedBaseOne = NULL;
  cachedBaseZero = NULL;
  cachedTensorOne = NULL;
  cachedTensorZero = NULL;

  //Clear the bddinfo_t vector.
  this->clear();

  //lock mutex
  numBddContexts--;
  if(numBddContexts == 0){
    //All BddContexts are now dead. So we must shutdown buddy.
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

void BddContext::setIntVars(const std::map<std::string, int>& vars)
{
  // First work through the variable list and create the vocabulary structure
  // This will collect information about the fdds to be created in buddy
  int * domains = new int[9 * vars.size()];    
  int vari = 0;
  for(std::map<std::string, int>::const_iterator ci = vars.begin(); ci != vars.end(); ++ci){
    if(ci->second < 2){
      *waliErr << "I haven't tested the library for int size less than 2";
      assert(false);
    }
    bddinfo_t varInfo = new BddInfo;
    varInfo->maxVal = ci->second;
    (*this)[ci->first] = varInfo;
    // fill in domains with the correct sizes for the fdds
    // There are 9 fdds corresponding to this variable with the size it->second
    for(int j=(vari)*9; j<9*(vari+1); ++j)
      domains[j] = ci->second;
    vari++;      
  }

  // Now actually create the fdd levels in buddy
  // lock mutex
  int base = fdd_extdomain(domains, 9*vars.size());
  if (base < 0){
    *waliErr << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl;
    *waliErr << "    Aborting." << endl;
    assert (false);
  }
  // release mutex
  delete domains;

  // Assign fdd levels to the variables.
  vari = 0;
  for(std::map<std::string, int>::const_iterator ci = vars.begin(); ci != vars.end(); ++ci){
    bddinfo_t varInfo = (*this)[ci->first];

    int j = 9 * vari;
    // The order of these assignments determines how the individual variables fdds are intermixed.
    varInfo->baseLhs = j++;
    varInfo->baseRhs = j++;
    varInfo->baseExtra = j++;
    varInfo->tensor1Lhs = j++;
    varInfo->tensor1Rhs = j++;
    varInfo->tensor1Extra = j++;
    varInfo->tensor2Lhs = j++;
    varInfo->tensor2Rhs = j++;
    varInfo->tensor2Extra = j++;
    assert(j == 9 * (vari + 1));

    // Also update the reverse vocabulary for printing.
    idx2Name[varInfo->baseLhs] = ci->first;
    idx2Name[varInfo->baseRhs] = ci->first + "'";
    idx2Name[varInfo->baseExtra] = ci->first + "''";
    idx2Name[varInfo->tensor1Lhs] = ci->first + "_t1";
    idx2Name[varInfo->tensor1Rhs] = ci->first + "_t1'";
    idx2Name[varInfo->tensor1Extra] = ci->first + "_t1''";
    idx2Name[varInfo->tensor2Lhs] = ci->first + "_t2";
    idx2Name[varInfo->tensor2Rhs] = ci->first + "_t2'";
    idx2Name[varInfo->tensor2Extra] = ci->first + "_t2''";

    vari++;  
  } 


  // Update bddPairs
  // We will first create arrays for each of columns
  int * baseLhs = new int[vars.size()];
  int * baseRhs = new int[vars.size()];
  int * baseExtra = new int[vars.size()];
  int * tensor1Lhs = new int[vars.size()];
  int * tensor1Rhs = new int[vars.size()];  
  int * tensor1Extra = new int[vars.size()];
  int * tensor2Lhs = new int[vars.size()];
  int * tensor2Rhs = new int[vars.size()];
  int * tensor2Extra = new int[vars.size()];
  vari = 0;
  for(std::map<std::string, int>::const_iterator ci = vars.begin(); ci != vars.end(); ++ci){
    bddinfo_t varInfo = (*this)[ci->first];
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
  fdd_setpairs(baseSwap.get(), baseLhs, baseRhs, vars.size());
  fdd_setpairs(baseSwap.get(), baseRhs, baseLhs, vars.size());
  fdd_setpairs(baseRightShift.get(), baseLhs, baseRhs, vars.size());
  fdd_setpairs(baseRightShift.get(), baseRhs, baseExtra, vars.size());
  fdd_setpairs(tensorRightShift.get(), tensor1Lhs, tensor1Rhs, vars.size());
  fdd_setpairs(tensorRightShift.get(), tensor2Lhs, tensor2Rhs, vars.size());
  fdd_setpairs(tensorRightShift.get(), tensor1Rhs, tensor1Extra, vars.size());
  fdd_setpairs(tensorRightShift.get(), tensor2Rhs, tensor2Extra, vars.size());
  fdd_setpairs(baseRestore.get(), baseExtra, baseRhs, vars.size());
  fdd_setpairs(tensorRestore.get(), tensor1Extra, tensor1Rhs, vars.size());
  fdd_setpairs(tensorRestore.get(), tensor2Extra, tensor2Rhs, vars.size());
  fdd_setpairs(move2Tensor1.get(), baseLhs, tensor1Lhs, vars.size());
  fdd_setpairs(move2Tensor1.get(), baseRhs, tensor1Rhs, vars.size());
  fdd_setpairs(move2Tensor2.get(), baseLhs, tensor2Lhs, vars.size());
  fdd_setpairs(move2Tensor2.get(), baseRhs, tensor2Rhs, vars.size());
  fdd_setpairs(move2Base.get(), tensor1Lhs, baseLhs, vars.size());
  fdd_setpairs(move2Base.get(), tensor2Rhs, baseRhs, vars.size());
  fdd_setpairs(move2BaseTwisted.get(), tensor1Rhs, baseLhs, vars.size());
  fdd_setpairs(move2BaseTwisted.get(), tensor2Rhs, baseRhs, vars.size());

  delete baseLhs;
  delete baseRhs;
  delete baseExtra;
  delete tensor1Lhs;
  delete tensor1Rhs;
  delete tensor1Extra;
  delete tensor2Lhs;
  delete tensor2Rhs;
  delete tensor2Extra;

  // Update static bdds
  // Somehow make this efficient
  for(std::map<std::string, int>::const_iterator ci = vars.begin(); ci != vars.end(); ++ci){
    bddinfo_t varInfo = (*this)[ci->first];
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
  }

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
  cachedTensorOne = new BinRel(this, tensorId, true);
  cachedTensorZero = new BinRel(this, bddfalse, true);
}

void BddContext::addIntVar(std::string name, unsigned size)
{
  //create and insert a new BddInfo into the vocabulary
  if(size < 2){
    *waliErr << "I haven't tested the library for int size less than 2";
    assert(false);
  }
  bddinfo_t varInfo = new BddInfo;
  varInfo->maxVal = size;
  (*this)[name] = varInfo;

  //Create space in buddy for this variable
  //We will create indices such that we get a default variable ordering where
  //baseLhs, baseRhs, baseExtra are mixed.
  //tensor1Lhs, tensor1Rhs, tensor1Extra are mixed.
  //tensor2Lhs, tensor2Rhs, tensor2Extra are mixed.

  //lock mutex
  int base;
  int domains[3] = {varInfo->maxVal, varInfo->maxVal, varInfo->maxVal};

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
  fdd_setpair(move2BaseTwisted.get(), varInfo->tensor1Rhs, varInfo->baseLhs);
  fdd_setpair(move2BaseTwisted.get(), varInfo->tensor2Rhs, varInfo->baseRhs);
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
  cachedTensorOne = new BinRel(this, tensorId, true);
  cachedTensorZero = new BinRel(this, bddfalse, true);
}


// ////////////////////////////
// Static
void BinRel::reset()
{
#ifdef BINREL_STATS
  //reset all counter;
  BinRel::numCompose = 0;
  BinRel::numUnion = 0;
  BinRel::numIntersect = 0;
  BinRel::numEqual = 0;
  BinRel::numKronecker = 0;
  BinRel::numTranspose = 0;
  BinRel::numEq23Project = 0;
  BinRel::numEq24Project = 0;
#endif
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

BinRel::BinRel(const BddContext * c, bdd b,bool it) : 
  con(c),
  rel(b), 
  isTensored(it) 
{}
BinRel::~BinRel() {}
// ////////////////////////////

binrel_t BinRel::Compose( binrel_t that ) const
{
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Composing incompatible relations" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con,bddfalse,isTensored);
  }
#endif
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
#ifdef BINREL_STATS
  BinRel::numCompose++;
#endif
  return new BinRel(con,c,isTensored);
}

binrel_t BinRel::Union( binrel_t that ) const
{
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Unioning incompatible relations" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con,bddtrue,isTensored);
  }
#endif
#ifdef BINREL_STATS
  BinRel::numUnion++;
#endif
  return new BinRel(con,rel | that->rel, isTensored);
}

binrel_t BinRel::Intersect( binrel_t that ) const
{
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Intersecting incompatible relations" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con,bddfalse,isTensored);
  }
#endif
#ifdef BINREL_STATS
  BinRel::numIntersect++;
#endif
  return new BinRel(con, rel & that->rel,isTensored);
}

bool BinRel::Equal( binrel_t that) const
{
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Compared (Equality) incompatible relations" 
      << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return false;
  }
#endif
#ifdef BINREL_STATS
  BinRel::numEqual++;
#endif
  return rel == that->rel;
}

binrel_t BinRel::Transpose() const
{
#ifndef BINREL_HASTY
  if(isTensored){
    *waliErr << "[WARNING] " << "Attempted to transpose tensored weight."
      << endl << "Not supported" << endl;
    print(*waliErr) << endl;
    assert(false);
    return new BinRel(con, bddfalse, true);
  }
#endif
  bdd c = bdd_replace(rel, con->baseSwap.get());
#ifdef BINREL_STATS
  BinRel::numTranspose++;
#endif
  return new BinRel(con, c, isTensored);
}

binrel_t BinRel::Kronecker(binrel_t that) const
{
#ifndef BINREL_HASTY
  if(isTensored || that->isTensored || con != that->con){
    *waliErr << "[WARNING] " << "Attempted to tensor two tensored weights OR attempted to tensor uncompatible relations."
      << endl << "Not supported" << endl;
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return new BinRel(con, bddfalse, true);
  }
#endif
  bdd rel1 = bdd_replace(rel, con->move2Tensor1.get());
  bdd rel2 = bdd_replace(that->rel, con->move2Tensor2.get());
  bdd c = rel1 & rel2;
#ifdef BINREL_STATS
  BinRel::numKronecker++;
#endif
  return new BinRel(con, c,true);
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
  bdd rel1 = rel & con->commonBddContextId23; 
  bdd rel2 = bdd_exist(rel1, con->commonBddContextSet23);
  bdd c = bdd_replace(rel2, con->move2Base.get());
#ifdef BINREL_STATS
  BinRel::numEq23Project++;
#endif
  return new BinRel(con,c,false);
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
  bdd rel1 = rel & con->commonBddContextId13; 
  bdd rel2 = bdd_exist(rel1, con->commonBddContextSet13);
  bdd c = bdd_replace(rel2, con->move2BaseTwisted.get());
#ifdef BINREL_STATS
  BinRel::numEq13Project++;
#endif
  return new BinRel(con,c,false);
}



// ////////////////////////////
// SemElem Interface functions

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
  return Eq23Project();
}

wali::sem_elem_tensor_t BinRel::detensorTranspose()
{
  return Eq13Project();
}

#ifdef BINREL_STATS
std::ostream& BinRel::printStats( std::ostream& o) const
{
  o << "BinRel Statistics:" <<endl;
  o << "Number of Compose operations: " << numCompose << endl; 
  o << "Number of Union operations: " << numUnion << endl; 
  o << "Number of Intersection operations: " << numIntersection << endl; 
  o << "Number of Equal operations: " << numEqual << endl; 
  return o;
}

#endif //BINREL_STATS


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

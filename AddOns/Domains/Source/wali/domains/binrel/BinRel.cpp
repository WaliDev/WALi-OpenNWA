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

#include <boost/algorithm/string/predicate.hpp>

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

BinRel::BinRel(BddContext const * c, bdd b,bool it) : 
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

///////////////////////////////

namespace wali {
  namespace domains {
    namespace binrel {
      
      typedef std::vector<std::pair<std::string, bddinfo_t> > VectorVocabulary;
      
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
      printImagemagickInstructions(bdd b, BddContext const & voc, std::ostream & os, std::string const & for_file)
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

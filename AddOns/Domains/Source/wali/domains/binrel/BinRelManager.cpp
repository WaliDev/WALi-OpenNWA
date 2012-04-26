#include "BinRelManager.hpp"

#include "glog/logging.h"
#include "buddy/fdd.h"

#include <cstdlib>
#include <ctime>

using namespace std;
using namespace  wali::domains::binrel;

// ////////////////////////////////////////
//     TRASH IN --- TRASH OUT            //
// ////////////////////////////////////////

using namespace wali::domains::binrel;

namespace wali
{
  namespace domains
  {
    namespace binrel
    {
      extern RevVoc idx2Name;
    }
  }
}

//TODO: remove this comment, and to do that, put comments elsewhere.


// ////////////////////////////////////////////////////////////////////////////
// The interface assumes that the google logging library is initialized before
// calling any of its functions.
namespace wali 
{
  namespace domains
  {
    namespace binrel
    {
      void addBoolVar(Voc& voc, std::string name)
      {
        bddinfo_t bi = new BddInfo;
        bi->maxVal = 2;
        voc[name] = bi;
      }

      void addIntVar(Voc& voc, std::string name, int size)
      {
        LOG_IF(WARNING, size < 2) 
          << "I haven't tested the library for int size less than 2";
        bddinfo_t bi = new BddInfo;
        bi->maxVal = size;
        voc[name] = bi;
      }
    } // namespace binrel
  } // namespace domains
} // namespace wali

BinRelManager::~BinRelManager()
{
  regAInfo = regBInfo = NULL;
  BinRel::reset();
}

BinRelManager::BinRelManager(Voc& voc, int bddMemSize, int cacheSize) :
  Countable(),
  BOOLSIZE(2),
  sizeInfo(-1),
  regAInfo(NULL),
  regBInfo(NULL)
{
  
  BinRel::initialize(voc, bddMemSize, cacheSize);

  //This stuff basically extends the bdd domain in buddy by some extra levels.
  {
    //Find the maximum register size in the vocabulary
    unsigned maxVal = 0;
    for(Voc::const_iterator vocIter = voc.begin();
        vocIter != voc.end();
        ++vocIter)
      maxVal = (vocIter->second->maxVal > maxVal)? vocIter->second->maxVal : maxVal;
    maxSize = maxVal;
    //Create space in the bdd to store the size of the current register.
    if(maxVal != 0){
      //if maxVal is zero, you can't do anything anyway.
      //+1 because I want to be able to store the size of the largest register.
      int domains[1] = {maxVal+1};
      int retSizeInfo = fdd_extdomain(domains,1);
      if(retSizeInfo < 0)
        LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(retSizeInfo) << "\"" << endl
          << "    Aborting." << endl;
      else
        sizeInfo = (unsigned) retSizeInfo;
      //To pretty print during testing, we add some names for this regsiter size fdd level.
      idx2Name[sizeInfo] = "__regSize";
      //Now create two extra bdd levels, and the corresponding BddInfo entries to be used for manipulation
      //inside BinRelManager.
      //We will create indices such that we get a default variable ordering where
      //baseLhs, baseRhs, baseExtra are mixed.
      {
        int domains[3] = {maxVal, maxVal, maxVal};
        regAInfo = new BddInfo();
        //Create fdds for base
        int base = fdd_extdomain(domains,3);
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;

        regAInfo->baseLhs = (unsigned) base;
        regAInfo->baseRhs = (unsigned) base + 1;
        regAInfo->baseExtra = (unsigned) base + 2;

        //To pretty print during testing, we add some names for this extra register
        //Currently, idx2Name is a global variable in wali::domains::binrel
        idx2Name[regAInfo->baseLhs] = "__regA";
        idx2Name[regAInfo->baseRhs] = "__regA'";
        idx2Name[regAInfo->baseExtra] = "__regA''";
      }
      {
        int domains[3] = {maxVal, maxVal, maxVal};
        regBInfo = new BddInfo();
        //Create fdds for base
        int base = fdd_extdomain(domains,3);
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        regBInfo->baseLhs = (unsigned) base;
        regBInfo->baseRhs = (unsigned )base + 1;
        regBInfo->baseExtra = (unsigned) base + 2;

        //To pretty print during testing, we add some names for this extra register
        //Currently, idx2Name is a global variable in wali::domains::binrel
        idx2Name[regBInfo->baseLhs] = "__regB";
        idx2Name[regBInfo->baseRhs] = "__regB'";
        idx2Name[regBInfo->baseExtra] = "__regB''";
      }
    }
  }
}

std::ostream& BinRelManager::print(std::ostream& o)
{
  o << "BinRelManager dump:" << std::endl;
  o << "sizeInfo: " << sizeInfo << std::endl;
  o << "maxSize: " << maxSize << std::endl;
  regAInfo->print(o << "regAInfo: ") << std::endl;
  regBInfo->print(o << "regBInfo: ") << std::endl;
  return o;
}

bdd BinRelManager::From(std::string var)
{
  //TODO: This header should be placed in all functions below.
  bdd ret = bddfalse;
  if(!BinRel::is_initialized()){
    LOG(ERROR) << "From called before initializing BinRel";
    return bddfalse;
  }
  const bddinfo_t bi = (*BinRel::getVoc().find(var)).second;
  for(unsigned i = 0; i < bi->maxVal; ++i)
    ret = ret | (fdd_ithvar(bi->baseLhs, i) & fdd_ithvar(regAInfo->baseRhs, i));
  return ret & fdd_ithvar(sizeInfo, bi->maxVal);
}

bdd BinRelManager::True()
{
  return fdd_ithvar(regAInfo->baseRhs,1) & fdd_ithvar(sizeInfo, BOOLSIZE);
}

bdd BinRelManager::False()
{
  return fdd_ithvar(regAInfo->baseRhs,0) & fdd_ithvar(sizeInfo, BOOLSIZE);
}

bdd BinRelManager::Const(unsigned val)
{
  if(val >= maxSize){
    LOG(ERROR) << "[Const] Attempted to create a constant value larger "
      << "than maxVal";
    return bddfalse;
  }
  return fdd_ithvar(regAInfo->baseRhs, val) & fdd_ithvar(sizeInfo, maxSize);
}

bdd BinRelManager::NonDet()
{
  return bddtrue;
}

bdd BinRelManager::applyBinOp(bdd lexpr, bdd rexpr, bdd op)
{ 
  bddPair *regA2regB = bdd_newpair();
  fdd_setpair(regA2regB, regAInfo->baseRhs, regBInfo->baseRhs);
  rexpr = bdd_replace(rexpr, regA2regB);
  bdd_freepair(regA2regB);

  lexpr = lexpr & rexpr;

  //IMP: fddsets are unioned by & of the representing bdds (Not | as I had
  //earlier expected.
  lexpr = bdd_relprod(
      lexpr,
      op, 
      fdd_ithset(regAInfo->baseRhs) & fdd_ithset(regBInfo->baseRhs)
      );

  bddPair *regAExtra2Rhs = bdd_newpair();
  fdd_setpair(regAExtra2Rhs, regAInfo->baseExtra, regAInfo->baseRhs);
  lexpr = bdd_replace(lexpr, regAExtra2Rhs);
  bdd_freepair(regAExtra2Rhs);

  return lexpr;
}

bdd BinRelManager::applyUnOp(bdd expr, bdd op)
{
  expr = bdd_relprod(
      expr,
      op,
      fdd_ithset(regAInfo->baseRhs)
      );

  bddPair *regAExtra2Rhs = bdd_newpair();
  fdd_setpair(regAExtra2Rhs, regAInfo->baseExtra, regAInfo->baseRhs);
  expr = bdd_replace(expr, regAExtra2Rhs);
  bdd_freepair(regAExtra2Rhs);

  return expr;
}

bdd BinRelManager::And(bdd lexpr, bdd rexpr)
{
  return applyBinOp(lexpr, rexpr, bddAnd());      
}

bdd BinRelManager::Or(bdd lexpr, bdd rexpr)
{
  return applyBinOp(lexpr, rexpr, bddOr());
}

bdd BinRelManager::Not(bdd expr)
{
  return applyUnOp(expr, bddNot());      
}

bdd BinRelManager::Plus(bdd lexpr, bdd rexpr)
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddPlus(in1,in2));
}

bdd BinRelManager::Minus(bdd lexpr, bdd rexpr)
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddMinus(in1,in2));
}

bdd BinRelManager::Times(bdd lexpr, bdd rexpr)
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddTimes(in1,in2));
}

bdd BinRelManager::Div(bdd lexpr, bdd rexpr)
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddDiv(in1,in2));
}

bdd BinRelManager::bddAnd()
{
  bdd ret =
    (fdd_ithvar(regAInfo->baseRhs,1) & 
     fdd_ithvar(regBInfo->baseRhs,1) &
     fdd_ithvar(regAInfo->baseExtra,1))
    |
    (fdd_ithvar(regAInfo->baseRhs,1) & 
     fdd_ithvar(regBInfo->baseRhs,0) &
     fdd_ithvar(regAInfo->baseExtra,0))
    |
    (fdd_ithvar(regAInfo->baseRhs,0) & 
     fdd_ithvar(regBInfo->baseRhs,1) &
     fdd_ithvar(regAInfo->baseExtra,0))
    |
    (fdd_ithvar(regAInfo->baseRhs,0) & 
     fdd_ithvar(regBInfo->baseRhs,0) &
     fdd_ithvar(regAInfo->baseExtra,0));
  ret = ret & fdd_ithvar(sizeInfo, BOOLSIZE);
  return ret;
}

bdd BinRelManager::bddOr()
{
  bdd ret = 
    (fdd_ithvar(regAInfo->baseRhs,1) & 
     fdd_ithvar(regBInfo->baseRhs,1) &
     fdd_ithvar(regAInfo->baseExtra,1))
    |
    (fdd_ithvar(regAInfo->baseRhs,1) & 
     fdd_ithvar(regBInfo->baseRhs,0) &
     fdd_ithvar(regAInfo->baseExtra,1))
    |
    (fdd_ithvar(regAInfo->baseRhs,0) & 
     fdd_ithvar(regBInfo->baseRhs,1) &
     fdd_ithvar(regAInfo->baseExtra,1))
    |
    (fdd_ithvar(regAInfo->baseRhs,0) & 
     fdd_ithvar(regBInfo->baseRhs,0) &
     fdd_ithvar(regAInfo->baseExtra,0));
  ret = ret & fdd_ithvar(sizeInfo, BOOLSIZE);
  return ret;
}

bdd BinRelManager::bddNot()
{
  bdd ret =
    (fdd_ithvar(regAInfo->baseRhs,1) & 
     fdd_ithvar(regAInfo->baseExtra,0))
    |
    (fdd_ithvar(regAInfo->baseRhs,0) & 
     fdd_ithvar(regAInfo->baseExtra,1));
  ret = ret & fdd_ithvar(sizeInfo, BOOLSIZE);
  return ret;
}

bdd BinRelManager::bddPlus(unsigned in1Size, unsigned in2Size)
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[BinRelManager::bddPlus] Addition of number of different bit widths is not allowed.\n";
  int outSize = in1Size;
  bdd ret = bddfalse;
  for(unsigned i=0; i<in1Size; ++i){
    for(unsigned j=0; j<in2Size; ++j){
      int k = (i + j) % outSize;
      ret = ret  |
        (fdd_ithvar(regAInfo->baseRhs,i) &
         fdd_ithvar(regBInfo->baseRhs,j) &
         fdd_ithvar(regAInfo->baseExtra,k));
    }
  }
  ret = ret & fdd_ithvar(sizeInfo, outSize);
  return ret;
}

bdd BinRelManager::bddMinus(unsigned in1Size, unsigned in2Size)
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[BinRelManager::bddMinus] Subtraction of number of different bit widths is not allowed.\n";
  int outSize = in1Size;
  bdd ret = bddfalse;
  for(unsigned i=0; i<in1Size; ++i){
    for(unsigned j=0; j<in2Size; ++j){
      int k = (i - j + outSize) % outSize;
      ret = ret  |
        (fdd_ithvar(regAInfo->baseRhs,i) &
         fdd_ithvar(regBInfo->baseRhs,j) &
         fdd_ithvar(regAInfo->baseExtra,k));
    }
  }
  ret = ret & fdd_ithvar(sizeInfo, outSize);
  return ret;
}

bdd BinRelManager::bddTimes(unsigned in1Size, unsigned in2Size)
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[BinRelManager::bddTimes] Multiplication of number of different bit widths is not allowed.\n";
  int outSize = in1Size;
  bdd ret = bddfalse;
  for(unsigned i=0; i<in1Size; ++i){
    for(unsigned j=0; j<in2Size; ++j){
      int k = (i * j) % outSize;
      ret = ret  |
        (fdd_ithvar(regAInfo->baseRhs,i) &
         fdd_ithvar(regBInfo->baseRhs,j) &
         fdd_ithvar(regAInfo->baseExtra,k));
    }
  }
  ret = ret & fdd_ithvar(sizeInfo, outSize);
  return ret;
}

bdd BinRelManager::bddDiv(unsigned in1Size, unsigned in2Size)
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[BinRelManager::bddDiv] Division of number of different bit widths is not allowed.\n";
  int outSize = in1Size;
  bdd ret = bddfalse;
  for(unsigned i=0; i<in1Size; ++i){
    for(unsigned j=0; j<in2Size; ++j){
      int k;
      if(j == 0)
        //arbitrary
        k = 0;
      else
        k = (int) i / j;
      ret = ret |
        (fdd_ithvar(regAInfo->baseRhs,i) &
         fdd_ithvar(regBInfo->baseRhs,j) &
         fdd_ithvar(regAInfo->baseExtra,k));
    }
  }
  ret = ret & fdd_ithvar(sizeInfo, outSize);
  return ret;
}

unsigned BinRelManager::getRegSize(bdd forThis)
{
  //Inefficient!!!
  for(unsigned i = 0; i <= maxSize; ++i){
    bdd tmp = fdd_ithvar(sizeInfo, i);
    if((tmp & forThis) != bddfalse)
      return i;
  }
  LOG(FATAL) << "[BinRelManager::getRegSize] bdd does not have a recognizable regSize\n";
  return 0;
}

bdd BinRelManager::Assign(std::string var, bdd expr)
{
  bddinfo_t bi;
  Voc voc = BinRel::getVoc();
  if(voc.find(var) == voc.end()){
    LOG(WARNING) << "[BinRelManager::Assign] Unknown Variable";
    return bddfalse;
  }else{
    bi = BinRel::getVoc().find(var)->second;
  }

  //redundant?
  bddPair *regARhs2Extra = bdd_newpair();
  fdd_setpair(
      regARhs2Extra,
      regAInfo->baseRhs,
      regAInfo->baseExtra
      );
  expr = bdd_replace(expr,regARhs2Extra);
  bdd_freepair(regARhs2Extra);
  //up to here.

  bdd regA2var = bddfalse;
  for(unsigned i = 0; i < bi->maxVal; ++i)
    regA2var = regA2var |
      (fdd_ithvar(regAInfo->baseExtra,i) & fdd_ithvar(bi->baseRhs,i));
  expr = bdd_relprod(expr,regA2var,fdd_ithset(regAInfo->baseExtra));

  bdd c = bddtrue;
  for(
      Voc::const_iterator iter = voc.begin();
      iter != voc.end();
      ++iter)
  {
    if(var != iter->first)
      c = c & fdd_equals((iter->second)->baseLhs,
          (iter->second)->baseRhs);
  }
  return bdd_exist(expr & c, fdd_ithset(sizeInfo));
}

bdd BinRelManager::Assume(bdd expr1, bdd expr2)
{
  const Voc voc = BinRel::getVoc();

  bddPair *regARhs2Extra = bdd_newpair();
  fdd_setpair(
      regARhs2Extra,
      regAInfo->baseRhs,
      regAInfo->baseExtra
      );
  expr1 = bdd_replace(expr1, regARhs2Extra);
  bdd_freepair(regARhs2Extra);

  bddPair *regARhs2BExtra = bdd_newpair();
  fdd_setpair(
      regARhs2BExtra,
      regAInfo->baseRhs,
      regBInfo->baseExtra
      );
  expr2 = bdd_replace(expr2, regARhs2BExtra);
  bdd_freepair(regARhs2BExtra);

  bddPair *baseLhs2Rhs = bdd_newpair();
  for(Voc::const_iterator iter = voc.begin(); iter != voc.end(); ++iter){
    fdd_setpair(
        baseLhs2Rhs,
        (iter->second)->baseLhs,
        (iter->second)->baseRhs
        );
  }
  expr2 = bdd_replace(expr2, baseLhs2Rhs);
  bdd_freepair(baseLhs2Rhs);

  bdd equate = bddtrue;
  for(Voc::const_iterator iter = voc.begin(); iter != voc.end(); ++iter){
    equate = equate &
      fdd_equals(
          iter->second->baseLhs,
          iter->second->baseRhs
          );
  } 
  equate = equate &
    fdd_equals(
        regAInfo->baseExtra,
        regBInfo->baseExtra
        );

  bdd ret = expr1 & expr2 & equate;

  ret = bdd_exist(ret, fdd_ithset(regAInfo->baseExtra));
  ret = bdd_exist(ret, fdd_ithset(regBInfo->baseExtra));

  return bdd_exist(ret, fdd_ithset(sizeInfo));
}

bdd BinRelManager::tGetRandomTransformer(bool isTensored)
{
  const Voc voc = BinRel::getVoc();
  bdd ret = bddfalse;
  int numRounds = rand() % 10 + 1;
  for(int count=0; count < numRounds; ++count){
    bdd inbdd = rand()%2?bddfalse:bddtrue;
    for(Voc::const_iterator iter = voc.begin(); 
        iter != voc.end();
        ++iter){
      int size = iter->second->maxVal;
      int n;
      if(!isTensored){
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->baseLhs,rand()%size);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->baseLhs,rand()%size);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->baseRhs,rand()%size);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->baseRhs,rand()%size);
      }else{
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor1Lhs,rand()%size);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor1Lhs,rand()%size);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor1Rhs,rand()%size);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor1Rhs,rand()%size);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor2Lhs,rand()%size);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor2Lhs,rand()%size);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor2Rhs,rand()%size);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor2Rhs,rand()%size);
      }
    }
    ret = rand() % 2 ? ret & inbdd : ret | inbdd;
  }
  return ret;
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

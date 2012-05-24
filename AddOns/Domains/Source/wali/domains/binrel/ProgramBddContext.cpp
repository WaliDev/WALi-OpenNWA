#include "ProgramBddContext.hpp"

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
      extern RevBddContext idx2Name;
    }
  }
}


namespace details
{
  bdd make_adder_box(int in1_varno, int in2_varno, int out_varno,
                     bdd t0, bdd t1, bdd t2, bool inv)
  {
    bdd a = bdd_ithvar(in1_varno);
    bdd b = bdd_ithvar(in2_varno);
    bdd c = bdd_ithvar(out_varno);

    if (inv) {
      // If there's a carry in, then the result is the opposite of what it
      // "should" be.
      c = !c;

      // Furthermore, if a+b=1, we carry to the next bit.
      t1 = t2;
    }

    // The following is a truth-table for a 1-bit adder, which also conjoins
    // in the appropriate box from the next level.
    return ((  a &  b & !c & t2)
            |( a & !b &  c & t1)
            |(!a &  b &  c & t1)
            |(!a & !b & !c & t0));
  }

  
  bdd make_adder(int fdd_lhs, int fdd_rhs, int fdd_result)
  {
    int num_vars = fdd_varnum(fdd_lhs);
    assert(num_vars == fdd_varnum(fdd_rhs));
    assert(num_vars == fdd_varnum(fdd_result));

    int * lhs_vars = fdd_vars(fdd_lhs);
    int * rhs_vars = fdd_vars(fdd_rhs);
    int * result_vars = fdd_vars(fdd_result);

    bdd left_box = bddtrue;
    bdd right_box = bddtrue;

    for (int level=0; level<num_vars; ++level) {
      // IMPORTANT: level counts from 0=msb to num_vars-1=lsb. However,
      // *_vars have index 0 as the lsb.
      int lhs_var = lhs_vars[num_vars-1 - level];
      int rhs_var = rhs_vars[num_vars-1 - level];
      int result_var = result_vars[num_vars-1 - level];

      bdd new_left_box = make_adder_box(lhs_var, rhs_var, result_var,
                                        left_box, left_box, right_box, false);
      bdd new_right_box = make_adder_box(lhs_var, rhs_var, result_var,
                                         left_box, right_box, right_box, true);

      left_box = new_left_box;
      right_box = new_right_box;
    }

    return left_box;
  }
}


// ////////////////////////////////////////////////////////////////////////////
// The interface assumes that the google logging library is initialized before
// calling any of its functions.

ProgramBddContext::~ProgramBddContext()
{
  regAInfo = regBInfo = NULL;
  BinRel::reset();
}

void ProgramBddContext::addBoolVar(std::string name)
{
  addIntVar(name,2);
}

void ProgramBddContext::addIntVar(std::string name, unsigned size)
{
  BddContext::addIntVar(name,size);
  if(size > maxSize){
    if(maxSize == 0){
      //This is when we *create* the extra levels needed
      int domains[1] = {size+1};
      int retSizeInfo = fdd_extdomain(domains,1);
      if(retSizeInfo < 0)
        LOG(FATAL) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(retSizeInfo) << "\"" << endl
          << "    Aborting." << endl;
      else
        sizeInfo = (unsigned) retSizeInfo;
      //To pretty print during testing, we add some names for this regsiter size fdd level.
      idx2Name[sizeInfo] = "__regSize";
      //Now create two extra bdd levels, and the corresponding BddInfo entries to be used for manipulation
      //inside ProgramBddContext.
      //We will create indices such that we get a default variable ordering where
      //baseLhs, baseRhs, baseExtra are mixed.
      {
        int domains2[3] = {size, size, size};
        regAInfo = new BddInfo();
        //Create fdds for base
        int base = fdd_extdomain(domains2,3);
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
        int domains2[3] = {size, size, size};
        regBInfo = new BddInfo();
        //Create fdds for base
        int base = fdd_extdomain(domains2,3);
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
    }else{
      //This is when we *enlarge* the extra levels needed
      int maxVal = size - maxSize;

      int domains[1] = {maxVal};
      int retSizeInfo = fdd_extdomain(domains,1);
      if(retSizeInfo < 0)
        LOG(FATAL) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(retSizeInfo) << "\"" << endl
          << "    Aborting." << endl;
      retSizeInfo = fdd_overlapdomain(sizeInfo,retSizeInfo);
      if(retSizeInfo < 0)
        LOG(FATAL) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(retSizeInfo) << "\"" << endl
          << "    Aborting." << endl;
      sizeInfo = retSizeInfo;

      //To pretty print during testing, we add some names for this regsiter size fdd level.
      idx2Name[sizeInfo] = "__regSize";
      //Now create two extra bdd levels, and the corresponding BddInfo entries to be used for manipulation
      //inside ProgramBddContext.
      //We will create indices such that we get a default variable ordering where
      //baseLhs, baseRhs, baseExtra are mixed.
      {
        int domains2[3] = {maxVal, maxVal, maxVal};
        //Create fdds for base
        int base = fdd_extdomain(domains2,3);
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        int retbase;
        retbase = fdd_overlapdomain(regAInfo->baseLhs,base);
        regAInfo->baseLhs = retbase;
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        retbase = fdd_overlapdomain(regAInfo->baseRhs,base+1);
        regAInfo->baseRhs = retbase;
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        retbase = fdd_overlapdomain(regAInfo->baseExtra,base+2);
        regAInfo->baseExtra = retbase;
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        //To pretty print during testing, we add some names for this extra register
        //Currently, idx2Name is a global variable in wali::domains::binrel
        idx2Name[regAInfo->baseLhs] = "__regA";
        idx2Name[regAInfo->baseRhs] = "__regA'";
        idx2Name[regAInfo->baseExtra] = "__regA''";
      }
      {
        int domains2[3] = {maxVal, maxVal, maxVal};
        //Create fdds for base
        int base = fdd_extdomain(domains2,3);
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        int retbase;
        retbase = fdd_overlapdomain(regBInfo->baseLhs,base);
        regBInfo->baseLhs = retbase;
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        retbase = fdd_overlapdomain(regBInfo->baseRhs,base+1);
        regBInfo->baseRhs = retbase;
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        retbase = fdd_overlapdomain(regBInfo->baseExtra,base+2);
        regBInfo->baseExtra = retbase;
        if (base < 0)
          LOG(ERROR) << "[ERROR-BuDDy initialization] \"" << bdd_errstring(base) << "\"" << endl
            << "    Aborting." << endl;
        //To pretty print during testing, we add some names for this extra register
        //Currently, idx2Name is a global variable in wali::domains::binrel
        idx2Name[regBInfo->baseLhs] = "__regB";
        idx2Name[regBInfo->baseRhs] = "__regB'";
        idx2Name[regBInfo->baseExtra] = "__regB''";
      }
    }
    maxSize = size;
  }
}

ProgramBddContext::ProgramBddContext(int bddMemSize, int cacheSize) :
  BddContext(bddMemSize, cacheSize),
  sizeInfo(0),
  maxSize(0),
  regAInfo(NULL),
  regBInfo(NULL)
{}

ProgramBddContext::ProgramBddContext(const ProgramBddContext& other) :
  BddContext(other),
  sizeInfo(other.sizeInfo),
  maxSize(other.maxSize),
  regAInfo(other.regAInfo),
  regBInfo(other.regBInfo)
{}

ProgramBddContext& ProgramBddContext::operator = (const ProgramBddContext& other)
{
  if(this != &other){
    BddContext::operator=(other);
    sizeInfo=other.sizeInfo;
    maxSize=other.maxSize;
    regAInfo=other.regAInfo;
    regBInfo=other.regBInfo;
  }
  return *this;
}

std::ostream& ProgramBddContext::print(std::ostream& o)
{
  o << "ProgramBddContext dump:" << std::endl;
  o << "sizeInfo: " << sizeInfo << std::endl;
  o << "maxSize: " << maxSize << std::endl;
  regAInfo->print(o << "regAInfo: ") << std::endl;
  regBInfo->print(o << "regBInfo: ") << std::endl;
  return o;
}

bdd ProgramBddContext::From(std::string var) const
{
  //TODO: This header should be placed in all functions below.
  bdd ret = bddfalse;
  ProgramBddContext::const_iterator iter = (this->find(var));
  if(iter == this->end())
    LOG(FATAL) << "Attempted From () on \"" << var << "\". I don't recognize this name\n";
  const bddinfo_t bi = iter->second;
  for(unsigned i = 0; i < bi->maxVal; ++i)
    ret = ret | (fdd_ithvar(bi->baseLhs, i) & fdd_ithvar(regAInfo->baseRhs, i));
  return ret & fdd_ithvar(sizeInfo, bi->maxVal);
}

bdd ProgramBddContext::True() const
{
  return fdd_ithvar(regAInfo->baseRhs,1) & fdd_ithvar(sizeInfo, 2);
}

bdd ProgramBddContext::False() const
{
  return fdd_ithvar(regAInfo->baseRhs,0) & fdd_ithvar(sizeInfo, 2);
}

bdd ProgramBddContext::Const(unsigned val) const
{
  if(val >= maxSize){
    LOG(ERROR) << "[Const] Attempted to create a constant value larger "
      << "than maxVal";
    return bddfalse;
  }
  return fdd_ithvar(regAInfo->baseRhs, val) & fdd_ithvar(sizeInfo, maxSize);
}

bdd ProgramBddContext::NonDet() const
{
  return bddtrue;
}

bdd ProgramBddContext::applyBinOp(bdd lexpr, bdd rexpr, bdd op) const
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

bdd ProgramBddContext::applyUnOp(bdd expr, bdd op) const
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

bdd ProgramBddContext::And(bdd lexpr, bdd rexpr) const
{
  return applyBinOp(lexpr, rexpr, bddAnd());      
}

bdd ProgramBddContext::Or(bdd lexpr, bdd rexpr) const
{
  return applyBinOp(lexpr, rexpr, bddOr());
}

bdd ProgramBddContext::Not(bdd expr) const
{
  return applyUnOp(expr, bddNot());      
}

bdd ProgramBddContext::Plus(bdd lexpr, bdd rexpr) const
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddPlus(in1,in2));
}

#ifdef BINREL_I_WANT_MINUS_TIMES_AND_DIV_EVEN_THOUGH_THEY_CAN_BE_EXPONENTIALLY_SLOW
bdd ProgramBddContext::Minus(bdd lexpr, bdd rexpr) const
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddMinus(in1,in2));
}

bdd ProgramBddContext::Times(bdd lexpr, bdd rexpr) const
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddTimes(in1,in2));
}

bdd ProgramBddContext::Div(bdd lexpr, bdd rexpr) const
{
  unsigned in1 = getRegSize(lexpr);
  unsigned in2 = getRegSize(rexpr);
  return applyBinOp(lexpr, rexpr, bddDiv(in1,in2));
}
#endif

bdd ProgramBddContext::bddAnd() const
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
  ret = ret & fdd_ithvar(sizeInfo, 2);
  return ret;
}

bdd ProgramBddContext::bddOr() const
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
  ret = ret & fdd_ithvar(sizeInfo, 2);
  return ret;
}

bdd ProgramBddContext::bddNot() const
{
  bdd ret =
    (fdd_ithvar(regAInfo->baseRhs,1) & 
     fdd_ithvar(regAInfo->baseExtra,0))
    |
    (fdd_ithvar(regAInfo->baseRhs,0) & 
     fdd_ithvar(regAInfo->baseExtra,1));
  ret = ret & fdd_ithvar(sizeInfo, 2);
  return ret;
}

bdd ProgramBddContext::bddPlus(unsigned in1Size, unsigned in2Size) const
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[ProgramBddContext::bddPlus] Addition of number of different bit widths is not allowed.\n";
  int outSize = in1Size;

#if BINREL_BUILD_FAST_ADDER  
  assert(in1Size <= (unsigned)fdd_domainsize(regAInfo->baseRhs));
  bdd plus_bdd = details::make_adder(regAInfo->baseRhs, regBInfo->baseRhs, regAInfo->baseExtra);
  plus_bdd = plus_bdd & fdd_ithvar(sizeInfo, outSize);
#endif
  
#if BINREL_BUILD_SLOW_ADDER
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
#endif

#if BINREL_BUILD_FAST_ADDER && BINREL_BUILD_SLOW_ADDER
  assert(plus_bdd == ret);  
#endif

#if BINREL_BUILD_FAST_ADDER
  return plus_bdd;
#elif BINREL_BUILD_SLOW_ADDER
  return ret;
#else
  #error "Must define one of BINREL_BUILD_FAST_ADDER or BINREL_BUILD_SLOW_ADDER"
#endif
}

#ifdef BINREL_I_WANT_MINUS_TIMES_AND_DIV_EVEN_THOUGH_THEY_CAN_BE_EXPONENTIALLY_SLOW
bdd ProgramBddContext::bddMinus(unsigned in1Size, unsigned in2Size) const
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[ProgramBddContext::bddMinus] Subtraction of number of different bit widths is not allowed.\n";
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

bdd ProgramBddContext::bddTimes(unsigned in1Size, unsigned in2Size) const
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[ProgramBddContext::bddTimes] Multiplication of number of different bit widths is not allowed.\n";
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

bdd ProgramBddContext::bddDiv(unsigned in1Size, unsigned in2Size) const
{
  if(in1Size != in2Size)
    LOG(ERROR) << "[ProgramBddContext::bddDiv] Division of number of different bit widths is not allowed.\n";
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
#endif

unsigned ProgramBddContext::getRegSize(bdd forThis) const
{
  //Inefficient!!!
  for(unsigned i = 0; i <= maxSize; ++i){
    bdd tmp = fdd_ithvar(sizeInfo, i);
    if((tmp & forThis) != bddfalse)
      return i;
  }
  LOG(FATAL) << "[ProgramBddContext::getRegSize] bdd does not have a recognizable regSize\n";
  return 0;
}

bdd ProgramBddContext::Assign(std::string var, bdd expr) const
{
  bddinfo_t bi;
  if(this->find(var) == this->end()){
    LOG(WARNING) << "[ProgramBddContext::Assign] Unknown Variable: " << var;
    return bddfalse;
  }else{
    bi = this->find(var)->second;
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
      BddContext::const_iterator iter = this->begin();
      iter != this->end();
      ++iter)
  {
    if(var != iter->first)
      c = c & fdd_equals((iter->second)->baseLhs,
          (iter->second)->baseRhs);
  }
  return bdd_exist(expr & c, fdd_ithset(sizeInfo));
}

bdd ProgramBddContext::Assume(bdd expr1, bdd expr2) const
{
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
  for(BddContext::const_iterator iter = this->begin(); iter != this->end(); ++iter){
    fdd_setpair(
        baseLhs2Rhs,
        (iter->second)->baseLhs,
        (iter->second)->baseRhs
        );
  }
  expr2 = bdd_replace(expr2, baseLhs2Rhs);
  bdd_freepair(baseLhs2Rhs);

  bdd equate = bddtrue;
  for(BddContext::const_iterator iter = this->begin(); iter != this->end(); ++iter){
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

bdd ProgramBddContext::tGetRandomTransformer(bool isTensored, unsigned seed)
{
  if(seed != 0)
    srand(seed);
  bdd ret = bddfalse;
  int numRounds = rand() % 10 + 1;
  for(int c=0; c < numRounds; ++c){
    bdd inbdd = rand()%2?bddfalse:bddtrue;
    for(BddContext::const_iterator iter = this->begin(); 
        iter != this->end();
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

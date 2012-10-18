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
  baseId = bddtrue;
  BinRel::reset();
}

void ProgramBddContext::addBoolVar(std::string name)
{
  addIntVar(name,2);
}

void ProgramBddContext::setIntVars(const std::map<std::string, int>& flatvars)
{
  std::vector<std::map<std::string, int> > vars;
  vars.push_back(flatvars);
  setIntVars(vars);
}


void ProgramBddContext::setIntVars(const std::vector<std::map<std::string, int> >& vars)
{
  BddContext::setIntVars(vars);
  
  // Compute the size of register needed.
  unsigned size = 0;
  for(std::map<const std::string, bddinfo_t>::const_iterator ci = this->begin(); ci != this->end(); ++ci)
    size = ci->second->maxVal > size ? ci->second->maxVal : size;
  size = size < 2 ? 2 : size; // The least size handled is 2. There must be at least one bool variable.
  maxSize = size;

  // Now create some buddy levels for scratchpad
  int domains[7] = {size + 1, size, size, size, size, size, size};
  int base = fdd_extdomain(domains,7);

  // Now create the corresponding bddinfo
  sizeInfo = (unsigned) base++;
  regAInfo = new BddInfo();
  regAInfo->baseLhs = (unsigned) base++;
  regAInfo->baseRhs = (unsigned) base++;
  regAInfo->baseExtra = (unsigned) base++;

  regBInfo = new BddInfo();
  regBInfo->baseLhs = (unsigned) base++;
  regBInfo->baseRhs = (unsigned) base++;
  regBInfo->baseExtra = (unsigned) base++;
  
  //To pretty print during testing, we add some names for this extra register
  //Currently, idx2Name is a global variable in wali::domains::binrel
  idx2Name[sizeInfo] = "__regSize";
  idx2Name[regAInfo->baseLhs] = "__regA";
  idx2Name[regAInfo->baseRhs] = "__regA'";
  idx2Name[regAInfo->baseExtra] = "__regA''";
  idx2Name[regBInfo->baseLhs] = "__regB";
  idx2Name[regBInfo->baseRhs] = "__regB'";
  idx2Name[regBInfo->baseExtra] = "__regB''";

  // Create cached identity
  baseId = bddtrue;
  int * baseLhs = new int[this->size()];
  int * baseRhs = new int[this->size()];
  int vari = 0;
  for(BddContextIter varIter = this->begin(); varIter != this->end();  ++varIter){
    bddinfo_t varInfo = (*varIter).second;
    baseId = baseId & fdd_equals(varInfo->baseLhs, varInfo->baseRhs);
    baseLhs[vari] = varInfo->baseLhs;
    baseRhs[vari] = varInfo->baseRhs;
    ++vari;
  }
  fdd_setpairs(baseLhs2Rhs.get(), baseLhs, baseRhs, this->size());
  delete [] baseLhs;
  delete [] baseRhs;
}

void ProgramBddContext::addIntVar(std::string name, unsigned siz)
{
  BddContext::addIntVar(name,siz);
  if(siz > maxSize){
    if(maxSize == 0){
      //This is when we *create* the extra levels needed
      int domains[1] = {siz+1};
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
        int domains2[3] = {siz, siz, siz};
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
        int domains2[3] = {siz, siz, siz};
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
      int maxVal = siz - maxSize;

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
    maxSize = siz;
  }

    //update cached identity and base pair
    bddinfo_t varInfo = this->find(name)->second;
    baseId = baseId & fdd_equals(varInfo->baseLhs, varInfo->baseRhs);
    fdd_setpair(baseLhs2Rhs.get(), varInfo->baseLhs, varInfo->baseRhs);
}

ProgramBddContext::ProgramBddContext(int bddMemSize, int cacheSize) :
  BddContext(bddMemSize, cacheSize),
  sizeInfo(0),
  maxSize(0),
  regAInfo(NULL),
  regBInfo(NULL),
  baseId(bddtrue),
  baseLhs2Rhs(BddPairPtr(bdd_newpair()))
{}

ProgramBddContext::ProgramBddContext(const ProgramBddContext& other) :
  BddContext(other),
  sizeInfo(other.sizeInfo),
  maxSize(other.maxSize),
  regAInfo(other.regAInfo),
  regBInfo(other.regBInfo),
  baseId(other.baseId),
  baseLhs2Rhs(other.baseLhs2Rhs)
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

std::ostream& ProgramBddContext::print(std::ostream& o) const
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
  bdd ret = bddfalse;
  ProgramBddContext::const_iterator iter = (this->find(var));
  if(iter == this->end())
    LOG(FATAL) << "Attempted From () on \"" << var << "\". I don't recognize this name\n";
  const bddinfo_t bi = iter->second;
  for(unsigned i = 0; i < bi->maxVal; ++i)
    ret = ret | (fdd_ithvar(bi->baseLhs, i) & fdd_ithvar(regAInfo->baseRhs, i));
  ret = bdd_exist(ret, fdd_ithset(sizeInfo));
  ret = ret & fdd_ithvar(sizeInfo, bi->maxVal);
  return ret;
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
    assert(0);
  }
  return fdd_ithvar(regAInfo->baseRhs, val) & fdd_ithvar(sizeInfo, maxSize);
}

bdd ProgramBddContext::NonDet() const
{
  bdd ret = bddtrue;
  //ret = bdd_exist(ret, fdd_ithset(sizeInfo));
  ret = ret & fdd_ithvar(sizeInfo, maxSize);
  return ret;
}

bdd ProgramBddContext::applyBinOp(bdd lexpr, bdd rexpr, bdd op) const
{ 
  unsigned old1Size = getRegSize(lexpr);
  unsigned old2Size = getRegSize(rexpr);
  // If the sizes are different, we don't want that part of
  // the bdd interfering in actual operation.
  if(old1Size != old2Size){
    lexpr = bdd_exist(lexpr, fdd_ithset(sizeInfo));
    rexpr = bdd_exist(rexpr, fdd_ithset(sizeInfo));
  }
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

  //update the regsize
  if(old1Size != old2Size){
    unsigned minSize = (old1Size < old2Size)? old1Size : old2Size;
    lexpr = lexpr & fdd_ithvar(sizeInfo, minSize);
    getRegSize(lexpr);
  }
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
  if(in1Size != in2Size){
    LOG(WARNING) << 
      "[ProgramBddContext::bddPlus] Different sizes of registers in operation." 
      << " Longer register / constant will be clipped.\n";
    if(in1Size < in2Size)
      in2Size = in1Size;
    else
      in1Size = in2Size;
  }
  unsigned outSize = in1Size;

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
  if(in1Size != in2Size){
    LOG(WARNING) << 
      "[ProgramBddContext::bddPlus] Different sizes of registers in operation." 
      << " Longer register / constant will be clipped.\n";
    if(in1Size < in2Size)
      in2Size = in1Size;
    else
      in1Size = in2Size;
  }
  unsigned outSize = in1Size;
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
  if(in1Size != in2Size){
    LOG(WARNING) << 
      "[ProgramBddContext::bddPlus] Different sizes of registers in operation." 
      << " Longer register / constant will be clipped.\n";
    if(in1Size < in2Size)
      in2Size = in1Size;
    else
      in1Size = in2Size;
  }
  unsigned outSize = in1Size;
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
  if(in1Size != in2Size){
    LOG(WARNING) << 
      "[ProgramBddContext::bddPlus] Different sizes of registers in operation." 
      << " Longer register / constant will be clipped.\n";
    if(in1Size < in2Size)
      in2Size = in1Size;
    else
      in1Size = in2Size;
  }
  unsigned outSize = in1Size;
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

  unsigned rvalsize = getRegSize(expr);
  if(rvalsize > bi->maxVal){
    LOG(WARNING) << "[ProgramBddContext::Assign] The register size of rhs is greater than what the variable can hold";
    return bddfalse;
  }

  // If rhs max size is smaller, only copy the relevant bits.
  unsigned copysize = bi->maxVal;
  copysize = (rvalsize < copysize) ? rvalsize : copysize;

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
  for(unsigned i = 0; i < copysize; ++i)
    regA2var = regA2var |
      (fdd_ithvar(regAInfo->baseExtra,i) & fdd_ithvar(bi->baseRhs,i));
  expr = bdd_relprod(expr,regA2var,fdd_ithset(regAInfo->baseExtra));

  // All transformers are slight perturbations of identity.
  bdd c = baseId;
  c = bdd_exist(c, fdd_ithset(bi->baseLhs));
  c = bdd_exist(c, fdd_ithset(bi->baseRhs));
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

  expr2 = bdd_replace(expr2, baseLhs2Rhs.get());

  bdd equate = baseId &
    fdd_equals(
        regAInfo->baseExtra,
        regBInfo->baseExtra
        );

  bdd ret = expr1 & expr2 & equate;

  ret = bdd_exist(ret, fdd_ithset(regAInfo->baseExtra));
  ret = bdd_exist(ret, fdd_ithset(regBInfo->baseExtra));

  return bdd_exist(ret, fdd_ithset(sizeInfo));
}

bdd ProgramBddContext::tGetRandomTransformer(bool isTensored, unsigned seed) const
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
      int siz = iter->second->maxVal;
      int n;
      if(!isTensored){
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->baseLhs,rand()%siz);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->baseLhs,rand()%siz);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->baseRhs,rand()%siz);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->baseRhs,rand()%siz);
      }else{
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor1Lhs,rand()%siz);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor1Lhs,rand()%siz);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor1Rhs,rand()%siz);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor1Rhs,rand()%siz);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor2Lhs,rand()%siz);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor2Lhs,rand()%siz);
        n = rand() % 4;
        if(n==0)
          inbdd = inbdd & fdd_ithvar(iter->second->tensor2Rhs,rand()%siz);
        if(n==1)    
          inbdd = inbdd | fdd_ithvar(iter->second->tensor2Rhs,rand()%siz);
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

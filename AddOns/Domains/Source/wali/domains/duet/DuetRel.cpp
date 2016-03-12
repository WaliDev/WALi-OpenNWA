
#include "DuetRel.hpp"

#include <iostream>
#include <sstream>
#include <cmath>

#include <boost/algorithm/string/predicate.hpp>

#include <tr1/unordered_map>

extern "C" {
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/alloc.h>
}

using namespace wali::domains::duetrel;
using std::endl;
using wali::waliErr;
using std::cout;

namespace wali
{
  namespace domains
  {
    namespace duetrel
    {

      duetrelpair_t DuetRelPair::MkDuetRelPair(duetrel_t v1, duetrel_t v2) {
          duetrelpair_t d = new DuetRelPair(v1, v2);
          return d;
      }




      static duetrel_t convert(wali::SemElem* se)
      {
          duetrel_t dr = dynamic_cast<DuetRel *>(se);
	  if (dr == NULL){
            *waliErr<<"[ERROR] Cannot cast to class duetrel::DuetRel.\n";
	    se->print (*waliErr << "    ") << endl;
	    assert(false);
          }
          return dr;
      }

      typedef std::pair< value, bool> StarCacheKey;
      struct StarCacheHash
      {
        size_t operator() (StarCacheKey k) const
        {
          return k.first << 1 & (int) k.second;
        }
      };
      std::tr1::unordered_map< StarCacheKey, sem_elem_t, StarCacheHash> star_cache;

    }
  }
}

// ////////////////////////////
// Static
void DuetRel::reset()
{
}

// ////////////////////////////
// Friends
namespace wali{
  namespace domains{
    namespace duetrel{
      duetrel_t operator*(duetrel_t a, duetrel_t b)
      {
        return a->Compose(b);
      }

      duetrel_t operator|(duetrel_t a, duetrel_t b)
      {
        return a->Union(b);
      }

      duetrel_t operator&(duetrel_t a, duetrel_t b)
      {
        return a->Intersect(b);
      }
    }
  }
}



int DuetRel::wCnt = 0;
value DuetRel::caml_weights[MAX_WEIGHT_COUNT];
// ////////////////////////////

// ////////////////////////////
// Members and Con/Destructors
DuetRel::DuetRel(const DuetRel& that):
  wali::SemElemTensor(that),
  relId(that.relId),
  isTensored(that.isTensored)
{
}

DuetRel::DuetRel(int b, bool it):
  relId(b),
  isTensored(it)
{
}
DuetRel::~DuetRel() {}
// ////////////////////////////

duetrel_t DuetRel::MkDuetRel(value w, bool tensored){
  CAMLparam1(w);
  static bool hasBeenInvoked = false;
  if (!hasBeenInvoked){
    hasBeenInvoked = true;
  }
  if (wCnt >= MAX_WEIGHT_COUNT)
  {
    ////abort
  }
  caml_register_global_root(&caml_weights[wCnt]);
  caml_weights[wCnt] = w;
  duetrel_t d = new DuetRel(wCnt, tensored);
  wCnt++;
  CAMLreturnT(duetrel_t,d);
}


value DuetRel::getValue() const
{
  CAMLparam0();
  CAMLlocal1(retVal);
  retVal = caml_weights[relId];
  CAMLreturn(retVal);
}

duetrel_t DuetRel::Compose( duetrel_t that ) const
{
  CAMLparam0();
  CAMLlocal3(dval0, dval1, retval);
  dval0 = this->getValue();
  dval1 = that->getValue();
  duetrel_t d;

  if (!isTensored){  
  value * com_func = caml_named_value("compose_callback");
  retval = caml_callback2(*com_func, dval0, dval1);

  d = MkDuetRel(retval);
  }
  else
  {
  value * com_func = caml_named_value("tensorCompose_callback");
  retval = caml_callback2(*com_func, dval0, dval1);

  d = MkDuetRel(retval, true);
  }
  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::Union( duetrel_t that ) const
{
  CAMLparam0();
  CAMLlocal3(dval0, dval1, retval);

  dval0 = this->getValue();
  dval1 = that->getValue();
  duetrel_t d;  

  if (!isTensored){
  value * union_func = caml_named_value("union_callback");
  retval = caml_callback2(*union_func, dval0, dval1);

  d = MkDuetRel(retval);
  }
  else
  {
  value * union_func = caml_named_value("tensorUnion_callback");
  retval = caml_callback2(*union_func, dval0, dval1);

  d = MkDuetRel(retval, true);
  }
  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::Intersect( duetrel_t that ) const
{
  CAMLparam0();
  std::cout << "INTERSECT!" << std::endl;
  duetrel_t d = MkDuetRel(that->getValue());
  CAMLreturnT(duetrel_t, d);
}

bool DuetRel::Equal( duetrel_t that) const
{
  CAMLparam0();
  CAMLlocal5(ret_d, dval0, nval0, nval1, dval1);

  dval0 = this->getValue();
  dval1 = that->getValue();
  //We skip this test if you insist
#ifndef BINREL_HASTY
  if(isTensored != that->isTensored){
    that->print(print(*waliErr) << endl) << endl;
    assert(false);
    return false;
  }
#endif
  value * norm_func = caml_named_value("normalize_callback");
  nval0 = caml_callback(*norm_func, dval0);
  nval1 = caml_callback(*norm_func, dval1);
  value * eq_func = caml_named_value("eq_callback");
  ret_d = caml_callback2(*eq_func, nval0, nval1);

  CAMLreturnT(bool,Bool_val(ret_d));
}

duetrel_t DuetRel::Transpose() const
{
  CAMLparam0();
  CAMLlocal2(dval, retval);

  dval = this->getValue();
  
  value * transpose_func = caml_named_value("transpose_callback");
  retval = caml_callback(*transpose_func, dval);

  duetrel_t d = MkDuetRel(retval);
  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::Kronecker(duetrel_t that) const
{
  CAMLparam0();
  CAMLlocal3(dval0, dval1, retval);

  dval0 = this->getValue();
  dval1 = that->getValue();
  
  value * tensor_func = caml_named_value("tensor_callback");
  retval = caml_callback2(*tensor_func, dval0, dval1);

  duetrel_t d = MkDuetRel(retval, true);
  CAMLreturnT(duetrel_t,d);
}


duetrel_t DuetRel::Merge(int v, int c) const
{
  CAMLparam0();
  CAMLlocal2(dval, retval);

  dval = this->getValue();

  value * merge_func;
  if (isTensored) {
    merge_func = caml_named_value("tensorMerge_callback");
  } else {
    merge_func = caml_named_value("merge_callback");
  }
  retval = caml_callback(*merge_func, dval);

  duetrel_t d = MkDuetRel(retval, isTensored);
  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::Normalize() const
{
  CAMLparam0();
  CAMLlocal2(dval, retval);
  
  dval = this->getValue();

  value * normalize_func = caml_named_value("normalize_callback");
  retval = caml_callback(*normalize_func, dval);

  duetrel_t d = MkDuetRel(retval);
  CAMLreturnT(duetrel_t, d);
}

duetrel_t DuetRel::TensorMerge(int v, int c) const
{
  CAMLparam0();
  CAMLlocal2(dval, retval);

  dval = this->getValue();
  value * tensorMerge_func = caml_named_value("tensorMerge_callback");
  retval = caml_callback(*tensorMerge_func, dval);

  duetrel_t d = MkDuetRel(retval, true);
  CAMLreturnT(duetrel_t, d);
}

// ////////////////////////////
// SemElem Interface functions

wali::sem_elem_t DuetRel::star()
{
  CAMLparam0();
  CAMLlocal2(dval, retVal);

  dval = this->getValue();
  duetrel_t d;
  
  if(!isTensored) {
  value * star_func = caml_named_value("star_callback");
  retVal = caml_callback(*star_func, dval);

  d = MkDuetRel(retVal);
  }
  else
  {
  value * star_func = caml_named_value("tensorStar_callback");
  retVal = caml_callback(*star_func, dval);

  d = MkDuetRel(retVal, true);
  }
  CAMLreturnT(duetrel_t,d);
}


duetrelpair_t DuetRel::alphaHatStar()
{
  CAMLparam0();
  CAMLlocal5(dval, temp, lin_formula, lin_formula_qelme, star_formula);

  dval = this->getValue();
  duetrel_t d1, d2;
  duetrelpair_t d;
  
  if (isTensored) {
 
    value * star_func = caml_named_value("tensor_linearize_star_callback");
    temp = caml_callback(*star_func, dval); 

    // Decompose temp into lin_formula and star_formula
    value * fst_func = caml_named_value("fst_callback");
    lin_formula = caml_callback(*fst_func, temp);
    // We call a quantifier elimination procedure on the linearized formula
    //   in preparation for later calling to DuetRel::Equivalent.
    value * qelme_func = caml_named_value("tensorQELME_callback");
    lin_formula_qelme = caml_callback(*qelme_func, lin_formula);
  } else {
    value * star_func = caml_named_value("linearize_star_callback");
    temp = caml_callback(*star_func, dval); 

    // Decompose temp into lin_formula and star_formula
    value * fst_func = caml_named_value("fst_callback");
    lin_formula = caml_callback(*fst_func, temp);
    // We call a quantifier elimination procedure on the linearized formula
    //   in preparation for later calling to DuetRel::Equivalent.
    value * qelme_func = caml_named_value("QELME_callback");
    lin_formula_qelme = caml_callback(*qelme_func, lin_formula);
  }
  
  value * snd_func = caml_named_value("snd_callback");
  star_formula = caml_callback(*snd_func, temp);

  d1 = MkDuetRel(lin_formula_qelme, isTensored);
  d2 = MkDuetRel(star_formula, isTensored);
  d = DuetRelPair::MkDuetRelPair(d1, d2);
  
  CAMLreturnT(duetrelpair_t,d);
}

bool DuetRel::Equivalent(duetrel_t that) const
{
  // We assume that a quantifier elimination procedure
  //   has been called on both arguments of the equivalence check.

  // I copied this call to CAMLparam0 from Equiv; I'm not sure
  //   if we should use CAMLparam0 or CAMLparam1 here.
  CAMLparam0();
  CAMLlocal3(retVal, dval0, dval1);

  dval0 = this->getValue();
  dval1 = that->getValue();
  
  if (isTensored) {
    value * eq_func = caml_named_value("tensorEquiv_callback");
    retVal = caml_callback2(*eq_func, dval0, dval1);
  } else {
    value * eq_func = caml_named_value("equiv_callback");
    retVal = caml_callback2(*eq_func, dval0, dval1);
  }
  // WARNING: The following print code causes segfaults.
  //   I haven't yet debugged the reason for this. -- Jason
  // std::cout << "Equivalence check of this:" << std::endl;
  // print(std::cout);
  // std::cout << std::endl << "against that:" << std::endl;
  // that->print(std::cout);
  // std::cout << std::endl << "Test result is:" << Bool_val(retVal) << std::endl;

  CAMLreturnT(bool,Bool_val(retVal));
}

wali::sem_elem_t DuetRel::combine(wali::SemElem* se) 
{
  duetrel_t that( convert(se) );
  return Union(that);
}

wali::sem_elem_t DuetRel::extend(wali::SemElem* se) 
{
  duetrel_t that( convert(se) );
  return Compose(that);
}

bool DuetRel::equal(wali::SemElem* se) const 
{
  duetrel_t that( convert(se) );
  return Equal(that);
}

bool DuetRel::containerLessThan(wali::SemElem const * se) const
{
  DuetRel const * other = dynamic_cast<DuetRel const *>(se);
  return this->getValue() < other->getValue();
}


wali::sem_elem_t DuetRel::one() const
{
  CAMLparam0();
  CAMLlocal1(oneV);

 value * one_func = caml_named_value("one_callback");
  oneV = caml_callback(*one_func, Val_unit);

  duetrel_t d = MkDuetRel(oneV);

  CAMLreturnT(duetrel_t,d);
}

wali::sem_elem_t DuetRel::zero() const
{
  CAMLparam0();
  CAMLlocal1(zeroV);
duetrel_t d;


	value * zero_func = caml_named_value("zero_callback");
  	zeroV = caml_callback(*zero_func, Val_unit);

  	d = MkDuetRel(zeroV);

  	CAMLreturnT(duetrel_t,d);
}

wali::sem_elem_t DuetRel::tensorZero() const
{
  CAMLparam0();
  CAMLlocal1(tzeroV);
  duetrel_t d;
 
	value * zero_func = caml_named_value("tensorZero_callback");
  	tzeroV = caml_callback(*zero_func, Val_unit);

  	d = MkDuetRel(tzeroV, true);

  CAMLreturnT(duetrel_t,d);

}

wali::sem_elem_t DuetRel::tensorOne() const
{
  CAMLparam0();
  CAMLlocal1(toneV);
  duetrel_t d;
 
	value * one_func = caml_named_value("tensorOne_callback");
  	toneV = caml_callback(*one_func, Val_unit);

  	d = MkDuetRel(toneV, true);

  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::getBaseOne()
{
  CAMLparam0();
  CAMLlocal1(oneV);
  duetrel_t d;
  
 	value * one_func = caml_named_value("one_callback");
  	oneV = caml_callback(*one_func, Val_unit);

  	d = MkDuetRel(oneV);


  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::getBaseZero()
{
  CAMLparam0();
  CAMLlocal1(zeroV);
  duetrel_t d;

	value * zero_func = caml_named_value("zero_callback");
  	zeroV = caml_callback(*zero_func, Val_unit);

  	d = MkDuetRel(zeroV);

  	CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::getTensorZero()
{
  CAMLparam0();
  CAMLlocal1(tzeroV);
  duetrel_t d;

	value * zero_func = caml_named_value("tensorZero_callback");
  	tzeroV = caml_callback(*zero_func, Val_unit);

  	d = MkDuetRel(tzeroV, true);

  CAMLreturnT(duetrel_t,d);
}

duetrel_t DuetRel::getTensorOne()
{
  CAMLparam0();
  CAMLlocal1(toneV);
  duetrel_t d;

	value * one_func = caml_named_value("tensorOne_callback");
  	toneV = caml_callback(*one_func, Val_unit);

  	d = MkDuetRel(toneV, true);


  CAMLreturnT(duetrel_t,d);
}

std::ostream& DuetRel::print( std::ostream& o ) const 
{
  CAMLparam0();
  CAMLlocal3(dval,sval,simpval);

  if(!isTensored)
    o << "Base relation: ";
  else
    o << "Tensored relation: ";


  dval = this->getValue();
  //value * norm_func = caml_named_value("normalize_callback");
  //ival = caml_callback(*norm_func,dval);
  if(!isTensored) {
  value * simplify_func = caml_named_value("simplify_callback");
  simpval = caml_callback(*simplify_func, dval);
  value * print_func = caml_named_value("print_callback");
  sval = caml_callback(*print_func, simpval);
  //sval = caml_callback(*print_func, dval);
  }
  else {
  value * simplify_func = caml_named_value("tensorSimplify_callback");
  simpval = caml_callback(*simplify_func, dval);
  value * print_func = caml_named_value("tensoredPrint_callback");
  sval = caml_callback(*print_func, simpval);
  //sval = caml_callback(*print_func, dval);
  }
  o << String_val(sval);
  CAMLreturnT(std::ostream&, o);
}

wali::sem_elem_tensor_t DuetRel::transpose() 
{
  return Transpose();
}

wali::sem_elem_tensor_t DuetRel::tensor(wali::SemElemTensor* se)
{
  duetrel_t that( convert(se) );
  return Kronecker(that);
}

wali::sem_elem_tensor_t DuetRel::detensor()
{
  assert(false); //This function, detensor, should never be called
#ifdef BINREL_STATS
  con->numDetensor++;
#endif
  return NULL;
}

wali::sem_elem_tensor_t DuetRel::detensorTranspose()
{
  CAMLparam0();
  CAMLlocal2(dval, retval);

  dval = this->getValue();
  value * detensorTranspose_func = caml_named_value("detensorTranspose_callback");
  retval = caml_callback(*detensorTranspose_func, dval);

  duetrel_t d = MkDuetRel(retval);

  CAMLreturnT(duetrel_t,d);
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

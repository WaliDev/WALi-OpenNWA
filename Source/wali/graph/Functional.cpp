#include "wali/graph/Functional.hpp"
#include "wali/graph/IntraGraph.hpp"

using namespace wali;
using namespace std;
using namespace wali::graph;

SemElemFunctional::SemElemFunctional() :
  type(TRASH),
  lhs(NULL),
  rhs(NULL),
  value(NULL),
  intra_nodeno(-1)
{}

SemElemFunctional::SemElemFunctional(sem_elem_tensor_t wt) :
  type(Constant),
  lhs(NULL),
  rhs(NULL),
  value(wt),
  intra_nodeno(-1)
{}

SemElemFunctional::SemElemFunctional(int n) :
  type(In),
  lhs(NULL),
  rhs(NULL),
  value(NULL),
  intra_nodeno(n)
{}


functional_t SemElemFunctional::constant(sem_elem_tensor_t wt)
{
  return new SemElemFunctional(wt);
}

functional_t SemElemFunctional::in(int n)
{
  return new SemElemFunctional(n);
}

functional_t SemElemFunctional::extend(functional_t lhs, functional_t rhs)
{
  functional_t res = new SemElemFunctional();
  res->type = Extend;
  res->lhs = lhs;
  res->rhs = rhs;
  return res;
}

functional_t SemElemFunctional::combine(functional_t lhs, functional_t rhs)
{
  functional_t res = new SemElemFunctional();
  res->type = Combine;
  res->lhs = lhs;
  res->rhs = rhs;
  return res;
}

functional_t SemElemFunctional::tensor(functional_t lhs, functional_t rhs)
{
  functional_t res = new SemElemFunctional();
  res->type = Tensor;
  res->lhs = lhs;
  res->rhs = rhs;
  return res;
}

functional_t SemElemFunctional::detensor(functional_t arg)
{
  functional_t res = new SemElemFunctional();
  res->type = Detensor;
  res->lhs = arg;
  return res;
}

functional_t SemElemFunctional::detensorTranspose(functional_t arg)
{
  functional_t res = new SemElemFunctional();
  res->type = DetensorTranspose;
  res->lhs = arg;
  return res;
}

functional_t SemElemFunctional::transpose(functional_t arg)
{
  functional_t res = new SemElemFunctional();
  res->type = Transpose;
  res->lhs = arg;
  return res;
}

sem_elem_tensor_t SemElemFunctional::evaluate(IntraGraph * const gr)
{
  sem_elem_tensor_t lval, rval, val;
  switch(type){
    case Constant:
      return value;
    case In:
      return dynamic_cast<SemElemTensor*>(gr->getWeight(intra_nodeno).get_ptr());
    case Extend:
       lval = lhs->evaluate(gr);
       rval = rhs->evaluate(gr);
      return dynamic_cast<SemElemTensor*>(lval->extend(rval.get_ptr()).get_ptr());
    case Combine:
       lval = lhs->evaluate(gr);
       rval = rhs->evaluate(gr);
      return dynamic_cast<SemElemTensor*>(lval->combine(rval.get_ptr()).get_ptr());
    case Tensor:
       lval = lhs->evaluate(gr);
       rval = rhs->evaluate(gr);
      return dynamic_cast<SemElemTensor*>(lval->tensor(rval.get_ptr()).get_ptr());
    case Detensor:
       val = lhs->evaluate(gr);
      return dynamic_cast<SemElemTensor*>(val->detensor().get_ptr());
    case DetensorTranspose:
       val = lhs->evaluate(gr);
      return dynamic_cast<SemElemTensor*>(val->detensorTranspose().get_ptr());
    case Transpose:
       val = lhs->evaluate(gr);
       return dynamic_cast<SemElemTensor*>(val->transpose().get_ptr());
    default:
      assert(false && "[SemElemFunctiona::evaluate] Unknown case\n");
  }
}

void SemElemFunctional::leafNodes(std::vector<int>& leaves)
{
  std::vector<int> ret;
  switch(type){
    case Constant:
      break;
    case In:
      leaves.push_back(intra_nodeno);
      break;
    case Extend:
    case Combine:
    case Tensor:
       lhs->leafNodes(leaves);
       rhs->leafNodes(leaves);
       break;
    case Detensor:
    case DetensorTranspose:
    case Transpose:
       lhs->leafNodes(leaves);
       break;
    default:
      assert(false && "[SemElemFunctiona::evaluate] Unknown case\n");
  }
}

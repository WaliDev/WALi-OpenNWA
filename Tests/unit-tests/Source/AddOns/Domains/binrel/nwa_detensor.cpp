
#include "gtest/gtest.h"
#include "buddy/bdd.h"

// ::std
#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <sstream>
#include <string>
#include <cstdlib>
#include <map>

#include <boost/cast.hpp>

#include <boost/static_assert.hpp>

// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/nwa_detensor.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"


using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::domains::binrel::details;
#if (NWA_DETENSOR == 1)
namespace
{
  TEST(Nwa_detensor, paperExample){
    program_bdd_context_t brm = new ProgramBddContext();
    map<string, int> vars;
    vars["x1"] = 2;
    vars["x2"] = 2;
    brm->setIntVars(vars);

    bdd b = brm->Assign("x1", brm->Not(brm->From("x1")));
    binrel_t M = new BinRel(brm.get_ptr(), b);
    binrel_t eye = boost::polymorphic_downcast<BinRel*>(M->one().get_ptr());
    binrel_t s = boost::polymorphic_downcast<BinRel*>(((M->tensor(M.get_ptr()))->combine(eye->tensor(eye.get_ptr()).get_ptr())).get_ptr());
    bdd z = s->getBdd();
    //bdd_fnprintdot_levels("mtmpiti.dot", z);
    binrel_t x = boost::polymorphic_downcast<BinRel*>(s->detensorTranspose().get_ptr());
    z = x->getBdd();
    ASSERT_TRUE(x->equal(x->one().get_ptr()));
    //bdd_fnprintdot_levels("detensored.dot", z);

    //b = brm->Assume(brm->From("x1"), brm->From("x1"));
    //bdd_fnprintdot_levels("id.dot", b);
  }

  TEST(Nwa_detensor, alltrue)
  {
    program_bdd_context_t brm = new ProgramBddContext();
    map<string, int> vars;
    vars["x1"] = 2;
    vars["x2"] = 2;
    vars["x3"] = 2;
    brm->setIntVars(vars);
    sem_elem_tensor_t b1 = new BinRel(brm.get_ptr(), bddtrue);
    sem_elem_tensor_t b2 = new BinRel(brm.get_ptr(), bddtrue);
    sem_elem_tensor_t b3 = b2->transpose();
    sem_elem_tensor_t b4 = b3->tensor(b1.get_ptr());
    sem_elem_tensor_t b5 = b4->detensorTranspose();
    sem_elem_tensor_t b6 = boost::polymorphic_downcast<SemElemTensor*>(b2->extend(boost::polymorphic_downcast<SemElem*>(b1.get_ptr())).get_ptr());
    ASSERT_TRUE(b6->equal(b5));
  }

  TEST(Nwa_detensor, failing1)
  {
    program_bdd_context_t brm = new ProgramBddContext();
    map<string, int> vars;
    vars["x1"] = 2;
    vars["x2"] = 2;
    vars["x3"] = 2;
    brm->setIntVars(vars);
    bdd b = brm->setPost("x2") & brm->setPre("x3") & brm->unsetPost("x3");
    sem_elem_tensor_t b1 = new BinRel(brm.get_ptr(), b);
    sem_elem_tensor_t b2 = new BinRel(brm.get_ptr(), b);
    sem_elem_tensor_t b3 = b2->transpose();
    sem_elem_tensor_t b4 = b3->tensor(b1.get_ptr());
    sem_elem_tensor_t b5 = b4->detensorTranspose();
    sem_elem_tensor_t b6 = boost::polymorphic_downcast<SemElemTensor*>(b2->extend(boost::polymorphic_downcast<SemElem*>(b1.get_ptr())).get_ptr());
    ASSERT_TRUE(b6->equal(b5));
  }

  TEST(Nwa_detensor, smallRandom)
  {
    program_bdd_context_t brm = new ProgramBddContext();
    map<string, int> vars;
    vars["x1"] = 2;
    vars["x2"] = 2;
    vars["x3"] = 2;
    vars["x4"] = 2;
    vars["x5"] = 2;
    brm->setIntVars(vars);
    for(unsigned i =0; i < 1000; ++i){
      sem_elem_tensor_t b1 = new BinRel(brm.get_ptr(), brm->tGetRandomTransformer(false, 0));
      sem_elem_tensor_t b2 = new BinRel(brm.get_ptr(), brm->tGetRandomTransformer(false, 0));
      sem_elem_tensor_t b3 = b2->transpose();
      sem_elem_tensor_t b4 = b3->tensor(b1.get_ptr());
      sem_elem_tensor_t b5 = b4->detensorTranspose();
      sem_elem_tensor_t b6 = boost::polymorphic_downcast<SemElemTensor*>(b2->extend(boost::polymorphic_downcast<SemElem*>(b1.get_ptr())).get_ptr());
      ASSERT_TRUE(b6->equal(b5));
    }
  }

}
#endif
// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

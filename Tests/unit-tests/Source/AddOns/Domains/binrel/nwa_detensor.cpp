
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

#include <boost/static_assert.hpp>

// ::wali::domains::binrel
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/nwa_detensor.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"


using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::domains::binrel::details;
#ifdef NWA_DETENSOR
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
    binrel_t eye = dynamic_cast<BinRel*>(M->one().get_ptr());
    binrel_t s = dynamic_cast<BinRel*>(((M->tensor(M.get_ptr()))->combine(eye->tensor(eye.get_ptr()).get_ptr())).get_ptr());
    bdd z = s->getBdd();
    //bdd_fnprintdot_levels("mtmpiti.dot", z);
    binrel_t x = dynamic_cast<BinRel*>(s->detensorTranspose().get_ptr());
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
    sem_elem_tensor_t b6 = dynamic_cast<SemElemTensor*>(b2->extend(dynamic_cast<SemElem*>(b1.get_ptr())).get_ptr());
    b1->print(cout << "b1: ") << endl;
    b2->print(cout << "b2: ") << endl;
    b3->print(cout << "b3: ") << endl;
    b4->print(cout << "b4: ") << endl;
    b5->print(cout << "b5: ") << endl;
    b6->print(cout << "b6: ") << endl;
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
    cout << "b: " << fddset << b << endl;
    sem_elem_tensor_t b1 = new BinRel(brm.get_ptr(), b);
    sem_elem_tensor_t b2 = new BinRel(brm.get_ptr(), b);
    sem_elem_tensor_t b3 = b2->transpose();
    sem_elem_tensor_t b4 = b3->tensor(b1.get_ptr());
    sem_elem_tensor_t b5 = b4->detensorTranspose();
    sem_elem_tensor_t b6 = dynamic_cast<SemElemTensor*>(b2->extend(dynamic_cast<SemElem*>(b1.get_ptr())).get_ptr());
    b1->print(cout << "b1: ") << endl;
    b2->print(cout << "b2: ") << endl;
    b3->print(cout << "b3: ") << endl;
    b4->print(cout << "b4: ") << endl;
    b5->print(cout << "b5: ") << endl;
    b6->print(cout << "b6: ") << endl;
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
      sem_elem_tensor_t b6 = dynamic_cast<SemElemTensor*>(b2->extend(dynamic_cast<SemElem*>(b1.get_ptr())).get_ptr());
      b1->print(cout << "b1: ") << endl;
      b2->print(cout << "b2: ") << endl;
      b3->print(cout << "b3: ") << endl;
      b4->print(cout << "b4: ") << endl;
      b5->print(cout << "b5: ") << endl;
      b6->print(cout << "b6: ") << endl;
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

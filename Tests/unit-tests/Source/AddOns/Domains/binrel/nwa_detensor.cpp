
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

namespace
{
  TEST(Nwa_detensor, printTest){
    program_bdd_context_t brm = new ProgramBddContext();
    map<string, int> vars;
    vars["x1"] = 2;
    vars["x2"] = 2;
    brm->setIntVars(vars);

    brm->setupLevelArray();
    BddContext::VocLevelArray const & lvls = brm->getLevelArray();
    for(BddContext::VocLevelArray::const_iterator cit = lvls.begin(); cit != lvls.end(); ++cit)
      cout << *cit << " ";
    cout << endl;
    bdd a = brm->Assign("x1", (brm->From("x1"))) & brm->Assume(brm->From("x1"), brm->Const(0));
    bdd b = brm->Assign("x2", (brm->From("x2"))) & brm->Assume(brm->From("x2"), brm->Const(0));
    bdd c = a & b; 
    binrel_t d = new BinRel(brm.get_ptr(), c); 
    binrel_t e = dynamic_cast<BinRel*>(d->tensor(d.get_ptr()).get_ptr());
   
    bdd z = e->getBdd();

    bdd_fnprintdot_levels("ttt.dot", z);
  }

}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

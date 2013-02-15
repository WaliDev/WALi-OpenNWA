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
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "wali/domains/binrel/BinRelMergeFns.hpp"


using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::domains::binrel::details;

namespace 
{
  TEST(wali$$domains$$binrel$$MeetMergeFn, oneLocalVarOnly)
  {
    ProgramBddContext con;
    map<string, int> vars;
    vars["l"] = 2;
    con.setIntVars(vars);

    vector<string> locals;
    locals.push_back("l");
    
    MeetMergeFn meet(&con,locals);
    
    sem_elem_t wt = new BinRel(&con, con.Assign("l", con.Const(0)));

    sem_elem_t res = meet.apply_f(wt,wt);

    EXPECT_TRUE(wt->equal(res));
    //wt->print(cout << "wt: ") << endl;
    //res->print(cout << "res: ") << endl;
  }
  
  TEST(wali$$domains$$binrel$$TensorMergeFn, oneLocalVarOnly)
  {
    ProgramBddContext con;
    map<string, int> vars;
    vars["l"] = 2;
    con.setIntVars(vars);

    vector<string> locals;
    locals.push_back("l");
    
    TensorMergeFn meet(&con,locals);
    
    sem_elem_t wt = new BinRel(&con, con.Assign("l", con.Const(0)));

    sem_elem_t res = meet.apply_f(wt,wt);

    EXPECT_TRUE(wt->equal(res));
    wt->print(cout << "wt: ") << endl;
    res->print(cout << "res: ") << endl;
  }
}

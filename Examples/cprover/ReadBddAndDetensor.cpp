// ::std
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
// ::wali::util
#include "wali/util/Timer.hpp"
#include "wali/domains/binrel/BinRel.hpp"
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "buddy/bdd.h"

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;

int main(int argc, char ** argv)
{
  if(argc < 2){
    cerr << "Usage: ./ReadBddAndDetensor bdd_context_file bdd_file" << endl;
    return 1;
  }

  ProgramBddContext * con;
  ifstream baz(argv[1]);
  con = ProgramBddContext::buildFromHistory(baz);
  baz.close();

  bdd a;
  bdd_fnload(argv[2], a);

  binrel_t b = new BinRel(con, a, true);

  wali::util::Timer * t = new wali::util::Timer("Detensor Time");
  t->measureAndReport = false;
  b->detensorTranspose();
  t->print(cout);
  delete t;

  return 0;
}

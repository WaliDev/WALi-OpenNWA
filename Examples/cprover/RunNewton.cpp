// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/EWPDS.hpp"
#if defined(USE_AKASH_EWPDS) || defined(USING_AKASH_FWPDS)
#include "wali/wpds/ewpds/ERule.hpp"
#endif
// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/TransFunctor.hpp"
// ::wali::wpds
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Rule.hpp"
// ::std
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
// ::wali
#include "wali/KeySpace.hpp"
#include "wali/Key.hpp"
#include "wali/ref_ptr.hpp"
// ::wali::util
#include "wali/util/Timer.hpp"
// ::wali::cprover
#include "BplToPds.hpp"

using namespace std;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::ewpds;
using namespace wali::wpds::fwpds;
using namespace wali::cprover;

int main(int argc, char ** argv)
{
  short dump = false;
  string fname;
  if(argc >=2){
    stringstream s;
    s << argv[1];
    s >> fname;
  }else{
    cerr << "Usage: ./NewtonFwpdsCompare input_file [<0/1> dump]\n";
    return -1;
  }
  if(argc >= 3){
    stringstream s;
    s << argv[2];
    s >> dump;
  }


  cout << "dump: " << dump << std::endl;

  {
    FWPDS * npds;
    wali::set_verify_fwpds(false);
    prog * pg = parse_prog(fname.c_str());
    npds = fwpds_from_prog(pg);
    npds->useNewton(true);
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    fa.setInitialState(getPdsState());
    fa.addFinalState(acc);
    fa.addTrans(getPdsState(),getEntryStk(pg, "main"), acc, npds->get_theZero()->one());

    if(dump){
      fstream newton_pds("newton_pds.dot", fstream::out);
      RuleDotty rd(newton_pds);
      newton_pds << "digraph{" << endl;
      npds->for_each(rd);
      newton_pds << "}" << endl;
    }
    if(dump){
      fstream innfa("newton_in_fa.dot", fstream::out);
      TransDotty td(innfa,false, NULL);
      innfa << "digraph{" << endl;
      fa.for_each(td);
      innfa << "}" << endl;
    }
    WFA outfa;
    {
      wali::util::Timer * t2 = new wali::util::Timer("NWPDS poststar",cout);
      cout << "[NWPDS poststar]\n";
      npds->poststar(fa,outfa);
      delete t2;
    }
    if(dump){
      fstream outfaf("newton_out_fa.dot", fstream::out);
      TransDotty td(outfaf,false, NULL);
      outfaf << "digraph{" << endl;
      outfa.for_each(td);
      outfaf << "}" << endl;
    }
    deep_erase_prog(&pg);
    delete npds;
  }
  return 0;
}

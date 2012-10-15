#include "BplToPds.hpp"

using namespace std;
using namespace wali::cprover;

int main(int argc, char ** argv)
{
  string fname;
  if(argc >= 2)
    fname = string(argv[1]);    
  else{
    fname = "satabs.3.bp";
  }

  prog * pg = parse_prog(fname.c_str());
  //WPDS * pds = wpds_from_prog(pg);
  
  print_prog_stats(pg);

  deep_erase_prog(&pg);
  return 0;
}

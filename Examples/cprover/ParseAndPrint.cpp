#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
// ::wali::cprover
#include "BplToPds.hpp"

using namespace std;
using namespace wali::cprover;


int main(int argc, char ** argv)
{
  char * mainProc = NULL, * errLbl = NULL;
  string fname;
  if(argc >=2){
    stringstream s;
    s << argv[1];
    s >> fname;
  }else{
    cerr << "Usage: ./ParseAndPrint input_file [entry function (default:main)] [error label (default:error)]\n";
    return -1;
  }
  if(argc >= 3){
    stringstream s;
    delete mainProc;
    mainProc = argv[2];
  }
  if(argc >= 4){
    stringstream s;
    delete errLbl;
    errLbl = argv[3];
  }
 
  cout << "Parsing program..." << endl;
  prog * pg = parse_prog(fname.c_str());
  
  cout << "Instrumenting asserts..." << endl;
  if(!mainProc) mainProc = strdup("main");
  if(!errLbl) errLbl = strdup("error");
  instrument_asserts(pg, errLbl);

  cout << "The program..." << endl;
  emit_prog(stdout, pg);

  deep_erase_prog(&pg);
  return 0;
}

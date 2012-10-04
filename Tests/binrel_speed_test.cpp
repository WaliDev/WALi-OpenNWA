#include "wali/domains/binrel/ProgramBddContext.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <map>

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;

int main(int argc, char ** argv)
{
  bool dbg = false;
  long totalCombine = 5e2;
  long extendPerCombine = 5e2;

  if(argc < 2)
    srand(time(NULL));
  else{
    int seed;
    istringstream (argv[1]) >> seed;
    srand(seed);
  }

  ProgramBddContext * voc = new ProgramBddContext();
  
  map<string, int > vars;
  vars["a"] = 2;
  vars["b"] = 2;
  vars["c"] = 2;
  vars["d"] = 2;
  vars["e"] = 2;
  vars["f"] = 2;
  vars["g"] = 2;
  vars["h"] = 2;
  voc->setIntVars(vars);
  
  sem_elem_tensor_t val = new BinRel(voc, bddfalse);
  val = val->tensor(val.get_ptr());
  sem_elem_tensor_t id = new BinRel(voc, voc->Assign("a", voc->From("a")));    
  for(long i = 0; i < totalCombine; ++i){
    sem_elem_tensor_t com = new BinRel(voc, voc->Assign("a", voc->From("a")));
    com = com->tensor(com.get_ptr());
    for(long j = 0; j < extendPerCombine; ++j){
      unsigned r = ((unsigned) rand()) % 9 + 1;
      string s;
      switch(r){
        case 1:
          s = "a";
          break;
        case 2:
          s = "b";
          break;
        case 3:
          s = "c";
          break;
        case 4:
          s = "c";
          break;
        case 5:
          s = "d";
          break;
        case 6: 
          s = "e";
          break;
        case 7:
          s = "f";
          break;
        case 8:
          s = "g";
          break;
        case 9:
          s = "h";
          break;
      }
      string ss = s;
      r = ((unsigned) rand()) % 9 + 1;
      switch(r){
        case 1:
          s = "a";
          break;
        case 2:
          s = "b";
          break;
        case 3:
          s = "c";
          break;
        case 4:
          s = "c";
          break;
        case 5:
          s = "d";
          break;
        case 6: 
          s = "e";
          break;
        case 7:
          s = "f";
          break;
        case 8:
          s = "g";
          break;
        case 9:
          s = "h";
          break;
      }
      sem_elem_tensor_t ext = new BinRel(voc, voc->Assign(s.c_str(), voc->From(ss.c_str()))); 
      r = ((unsigned) rand()) % 100;
      if(dbg) cout << "r: " << r << endl;
      if(r < 4)
        ext = ext->tensor(id.get_ptr());
      else
        ext = id->tensor(ext.get_ptr());
      com = dynamic_cast<SemElemTensor*>(com->extend(ext.get_ptr()).get_ptr());
      if(dbg) cout << "Next extend" << endl;
    }
    val = dynamic_cast<SemElemTensor*>(val->combine(com.get_ptr()).get_ptr());
    if(dbg) cout << "Next combine" << endl;
  }

  val = NULL;

  delete voc;

  cout << "Done!" << endl;
}

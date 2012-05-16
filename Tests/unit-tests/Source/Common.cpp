
// ::std
#include <string>
#include <sstream>
#include <fstream>

namespace{
  std::string regrDir = "Tests/harness/unit-tests/regression_baseline";

  //declarations
  static void writeOutput(
      std::string testname,
      std::string varname, 
      std::stringstream& ss
      );

  //definitions
  static void writeOutput(
      std::string testname,
      std::string varname, 
      std::stringstream& ss
      )
  {
    std::string outpath = testname + "_" + varname + ".output";
    std::fstream fout(outpath.c_str(),std::ios_base::out);
    fout << ss.str();
  };

  bool compareOutput(
      std::string testname,
      std::string varname,
      std::stringstream& outs
      )
  {
    writeOutput(testname, varname, outs);
    std::string inpath = regrDir + "/" + testname + "_" + varname + ".output";
    cout << inpath << endl;
    std::fstream fin(inpath.c_str(),std::ios_base::in);
    std::stringstream ins;
    std::string s;
    char c;
    while(fin.get(c)){
      ins.put(c);
    }
   
    bool res = ins.str() == outs.str();
    if(!res){
      cout << "Difference found!!!\n";
      cout << "ins.str(): " << ins.str() << "\n";;
      cout << "outs.str(): " << outs.str() << "\n";
    }
    return res;
  };
}

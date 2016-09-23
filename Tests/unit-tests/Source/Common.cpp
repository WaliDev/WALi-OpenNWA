
// ::std
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>

namespace{
  //std::string regrDir = "Tests/harness/unit-tests/regression_baseline";
  std::string regrDir = "regression_baseline";

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
    if (!fin) {
      cout << "Could not open " << inpath << "\n";
      return false;
    }
    std::stringstream ins;
    std::string s;
    char c;
    while(fin.get(c)){
      ins.put(c);
    }
  

    std::string inc = ins.str();
    inc.erase(std::remove_if(inc.begin(), inc.end(), ::isspace), inc.end());
    std::string outc = outs.str();
    outc.erase(std::remove_if(outc.begin(), outc.end(), ::isspace), outc.end());
    bool res = (inc == outc);
    if(!res){
      cout << "Difference found!!!\n";
      cout << "expected.str(): " << ins.str() << "\n";;
      cout << "obtained.str(): " << outs.str() << "\n";
    }
    return res;
  };
}

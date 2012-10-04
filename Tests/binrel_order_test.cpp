#include "wali/domains/binrel/ProgramBddContext.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;

ProgramBddContext * voc;

void print_bddlevels(string var)
{
  int *baseLhs, *baseRhs, *baseExtra, *tensor1Lhs, *tensor1Rhs, *tensor1Extra, *tensor2Lhs, *tensor2Rhs, *tensor2Extra;
  baseLhs = fdd_vars((*voc)[var]->baseLhs);
  baseRhs = fdd_vars((*voc)[var]->baseRhs);
  baseExtra = fdd_vars((*voc)[var]->baseExtra);
  tensor1Lhs = fdd_vars((*voc)[var]->tensor1Lhs);
  tensor1Rhs = fdd_vars((*voc)[var]->tensor1Rhs);
  tensor1Extra = fdd_vars((*voc)[var]->tensor1Extra);
  tensor2Lhs = fdd_vars((*voc)[var]->tensor2Lhs);
  tensor2Rhs = fdd_vars((*voc)[var]->tensor2Rhs);
  tensor2Extra = fdd_vars((*voc)[var]->tensor2Extra);

  cout << var << "1: " << baseLhs[0] << " ";
  cout << var << "2: " << baseLhs[1] << " ";
  cout << var << "1': " << baseRhs[0] << " ";
  cout << var << "2': " << baseRhs[1] << " ";
  cout << var << "1'': " << baseExtra[0] << " ";
  cout << var << "2'': " << baseExtra[1] << " ";
  cout << endl;

  cout << var << "1t1: " << tensor1Lhs[0] << " ";
  cout << var << "2t1: " << tensor1Lhs[1] << " ";
  cout << var << "1t1': " << tensor1Rhs[0] << " ";
  cout << var << "2t1': " << tensor1Rhs[1] << " ";
  cout << var << "1t1'': " << tensor1Extra[0] << " ";
  cout << var << "2t1'': " << tensor1Extra[1] << " ";
  cout << endl;

  cout << var << "1t2: " << tensor2Lhs[0] << " ";
  cout << var << "2t2: " << tensor2Lhs[1] << " ";
  cout << var << "1t2': " << tensor2Rhs[0] << " ";
  cout << var << "2t2': " << tensor2Rhs[1] << " ";
  cout << var << "1t2'': " << tensor2Extra[0] << " ";
  cout << var << "2t2'': " << tensor2Extra[1] << " ";
  cout << endl << endl;
}

int main(int argc, char ** argv)
{
  voc = new ProgramBddContext();
  vector<map<string, int> > vars;
  map<string, int > vars1;
  map<string, int > vars2;
  vars1["a"] = 4;
  vars1["b"] = 4;
  vars2["c"] = 4;
  vars2["d"] = 4;
  vars.push_back(vars1);
  vars.push_back(vars2);
  voc->setIntVars(vars);
 
  print_bddlevels("a");
  print_bddlevels("b");
  print_bddlevels("c");
  print_bddlevels("d");

  delete voc;

  return 0;
} 

#include "wali/nwa/NWA.hpp"
#include <iostream>

using namespace std;

int main()
{
  wali::nwa::SymbolSet s;
  s.addSymbol(wali::getKey("hello"));
  cout << s.isSymbol(wali::getKey("hello")) << endl;
}

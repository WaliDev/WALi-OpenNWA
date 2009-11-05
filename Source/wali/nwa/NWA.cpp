/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/nwa/NWA.hpp"
#include "wali/nwa/State.hpp"
#include "wali/nwa/Symbol.hpp"
#include "wali/Key.hpp"

int main()
{

  wali::nwa::NWA<wali::nwa::State<wali::Key>,wali::nwa::Symbol<wali::Key>> myNWA;
  
  //myNWA.print(std::cout);

return 0;
}
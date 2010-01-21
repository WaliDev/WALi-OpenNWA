/*!
 * @author Nick Kidd
 *
 */

#include "wali/wfa/WFA.hpp"
// Reachability weight domain
// $(WaliDir)/Examples/Reach/Reach.hpp
#include "Reach.hpp"
#include <iostream>

int main()
{
  using wali::wfa::WFA;
  using wali::Key;
  using wali::getKey;
  using std::cout;

  WFA fa;
  fa.addTrans( getKey("p"),getKey("n1"),getKey("accept"),new Reach(true) );
  fa.setInitialState( getKey("p") );
  fa.addFinalState( getKey("accept") );
  fa.print( cout );
  fa.prune();
  fa.print( cout );
  cout << "\n------------------------------------------------------------\n\n";

  fa.addTrans( getKey("p"),getKey("n2"),getKey("BAD"),new Reach(true) );
  fa.print( cout );
  fa.prune();
  fa.print( cout );
  cout << "\n------------------------------------------------------------\n\n";

  fa.addTrans( getKey("p"),getKey("n2"),getKey("BAD"),new Reach(true) );
  fa.addTrans( getKey("BAD"),getKey("n2"),getKey("BAD"),new Reach(true) );
  fa.addTrans( getKey("BAD"),getKey("n3"),getKey("BAD2"),new Reach(true) );
  fa.print( cout );
  fa.prune();
  fa.print( cout );
  cout << "\n------------------------------------------------------------\n\n";
  return 0;
}


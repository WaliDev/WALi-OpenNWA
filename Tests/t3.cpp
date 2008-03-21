/*!
 * @author Nick Kidd
 *
 * Tests printing WFA in dot format and marshalling to XML.
 */
#include <iostream>
#include <fstream>
#include "wali/Common.hpp"
#include "wali/wfa/WFA.hpp"
// For debug info in main()
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"

// Reachability Weight Domain
#include "Reach.hpp"

//
// n0:
//      if( ?? )
// n1:      x
//      else
// n2:      y
// n3:  ...
//
void dot()
{
  using wali::wpds::WPDS;
  using wali::getKey;
  using wali::Key;
  using wali::sem_elem_t;
  using wali::wfa::WFA;
  using std::cout;
  sem_elem_t R = new Reach(true);

  Key p = getKey("p");
  Key acc1 = getKey("acc1");
  Key acc2 = getKey("acc2");
  Key n0 = getKey("n0");
  Key n1 = getKey("n1");
  Key n2 = getKey("n2");
  Key n3 = getKey("n3");

  WFA fa;
  fa.setInitialState(p);
  fa.addFinalState(acc1);
  fa.addFinalState(acc2);
  fa.addTrans( p, getKey("n0"), acc1, R->one());
  fa.addTrans( p, n0, p, R->one());
  fa.addTrans( p, n1, getKey("q"), R->one() );
  fa.addTrans( getKey("q"), n2, acc2, R->one());
  fa.addTrans( acc2, n3, acc2, R->one());
  std::ofstream fdot( "t3.dot" );
  fa.print_dot( fdot,false );
  fdot.close();

  std::ofstream fxml( "t3.xml" );
  fa.marshall( fxml );
  fxml.close();

}

int main()
{
  dot();
  std::cerr << "# Trans : " << wali::wfa::Trans::numTrans << std::endl;
  std::cerr << "# States : " << wali::wfa::State::numStates << std::endl;
  std::cerr << "# Reaches : " << Reach::numReaches << std::endl;
  return 0;
}


/*!
 * @author Nick Kidd
 *
 * $Id: t4.cpp 418 2008-05-12 17:19:03Z kidd $
 *
 * Tests path_summary
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
  using wali::getKey;;
  using wali::wali_key_t;
  using wali::sem_elem_t;
  using wali::wfa::WFA;
  using std::cout;
  sem_elem_t R = new Reach(true);

  wali_key_t p = getKey("p");
  wali_key_t acc1 = getKey("acc1");
  wali_key_t acc2 = getKey("acc2");
  wali_key_t n0 = getKey("n0");
  wali_key_t n1 = getKey("n1");
  wali_key_t n2 = getKey("n2");
  wali_key_t n3 = getKey("n3");

  WFA fa;
  fa.set_initial_state(p);
  fa.add_final_state(acc1);
  fa.add_final_state(acc2);
  fa.addTrans( p, getKey("n0"), acc1, R->one());
  fa.addTrans( p, n0, p, R->one());
  fa.addTrans( p, n1, getKey("q"), R->one());
  fa.addTrans( getKey("q"), n2, acc2, R->one());
  fa.addTrans( acc2, n3, acc2, R->one());

  fa.path_summary();
}

int main()
{
  dot();
  std::cerr << "# Trans   : " << wali::wfa::Trans::numTrans << std::endl;
  std::cerr << "# States  : " << wali::wfa::State::numStates << std::endl;
  std::cerr << "# Reaches : " << Reach::numReaches << std::endl;
  return 0;
}


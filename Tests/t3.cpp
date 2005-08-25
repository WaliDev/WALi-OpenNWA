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
#include "Examples/Reach/Reach.hpp"

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
    using wali::wali_key_t;
    using wali::sem_elem_t;
    using wali::wfa::WFA;
    using std::cout;
    Reach *reach = new Reach(true);

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
    fa.add_trans( p, getKey("n0"), acc1, reach);
    fa.add_trans( p, n0, p, reach);
    fa.add_trans( p, n1, getKey("q"), reach);
    fa.add_trans( getKey("q"), n2, acc2, reach);
    fa.add_trans( acc2, n3, acc2, reach);
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

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

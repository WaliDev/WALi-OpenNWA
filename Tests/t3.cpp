/*!
 * @author Nick Kidd
 *
 * Tests printing WFA in dot format and marshalling to XML.
 */
#include <iostream>
#include <fstream>
#include "wali/KeyFactory.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wpds/WitnessWPDS.hpp"
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
    using wali::wpds::WitnessWPDS;
    using wali::wpds::WPDS;
    using wali::KeyFactory;
    using wali::wali_key_t;
    using wali::sem_elem_t;
    using wali::wfa::WFA;
    using std::cout;
    Reach *reach = new Reach(true);

    wali_key_t p = KeyFactory::get_key("p");
    wali_key_t acc1 = KeyFactory::get_key("acc1");
    wali_key_t acc2 = KeyFactory::get_key("acc2");
    wali_key_t n0 = KeyFactory::get_key("n0");
    wali_key_t n1 = KeyFactory::get_key("n1");
    wali_key_t n2 = KeyFactory::get_key("n2");
    wali_key_t n3 = KeyFactory::get_key("n3");

    WFA fa;
    fa.set_initial_state(p);
    fa.add_final_state(acc1);
    fa.add_final_state(acc2);
    fa.add_trans( p, KeyFactory::get_key("n0"), acc1, reach);
    fa.add_trans( p, n0, p, reach);
    fa.add_trans( p, n1, KeyFactory::get_key("q"), reach);
    fa.add_trans( KeyFactory::get_key("q"), n2, acc2, reach);
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

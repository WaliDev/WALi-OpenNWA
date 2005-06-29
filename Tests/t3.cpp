/*!
 * $Id: t3.cpp,v 1.2 2005/06/23 17:11:25 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.2 $
 *
 * Tests printing WFA in dot format.
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

    fa.marshall( std::cout );

}

int main()
{
    dot();
    std::cerr << "# Trans : " << wali::wfa::Trans::numTrans << std::endl;
    std::cerr << "# States : " << wali::wfa::State::numStates << std::endl;
    std::cerr << "# Reaches : " << Reach::numReaches << std::endl;
    return 0;
}

/*
 * $Log: t3.cpp,v $
 * Revision 1.2  2005/06/23 17:11:25  kidd
 * Add fa.marshall(std::cout) to t3 to compare XML with dot output
 *
 * Revision 1.1  2005/06/21 23:55:35  kidd
 * Added t3.cpp to test WFA::print_dot
 *
 * Revision 1.3  2005/06/17 21:09:19  kidd
 * Add more paths in WitnessWPDS to generate a WitnessCombine
 *
 * Revision 1.2  2005/06/17 02:42:05  kidd
 * Made pds instance be of static ttype WitnessWPDS
 *
 * Revision 1.1  2005/06/10 18:56:00  kidd
 * Witness test code
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


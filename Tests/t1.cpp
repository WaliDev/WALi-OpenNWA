/*!
 * $Id: t1.cpp,v 1.2 2005/06/10 18:51:26 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.2 $
 */
#include <iostream>
#include "wali/KeyFactory.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/WeightMaker.hpp"
#include "Examples/Reach/Reach.hpp"

void dot()
{
    using wali::KeyFactory;
    using wali::wali_key_t;
    using wali::sem_elem_t;
    using wali::wfa::WFA;
    using std::cout;
    Reach *r = new Reach(true);

    // p - n1 - q
    WFA fa1;
    wali_key_t p = KeyFactory::get_key("p");
    wali_key_t q = KeyFactory::get_key("q");
    fa1.set_initial_state( p );
    fa1.add_final_state( q );
    fa1.add_trans( p, KeyFactory::get_key("n1"), q, r);

    fa1.print( cout << "WFA 1\n" ) << std::endl;

    // m - n1 - q 
    WFA fa2;
    wali_key_t m = KeyFactory::get_key("m");
    fa2.set_initial_state( m );
    fa2.add_final_state( q );
    fa2.add_trans( m,KeyFactory::get_key("n1"),q,r);
    fa2.print( cout << "WFA 2\n" ) << std::endl;


    fa1.intersect( fa2 ).print( cout << "1 \\cap 2\n" ) << std::endl;

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
 * $Log: t1.cpp,v $
 * Revision 1.2  2005/06/10 18:51:26  kidd
 * Rename doit to dot
 *
 * Revision 1.1  2005/06/07 19:00:19  kidd
 *
 * t1.cpp tests WFA intersection
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


/*!
 * @author Nick Kidd
 *
 * Tests Witnesses
 */
#include <iostream>
#include "wali/Common.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/witness/WitnessWrapper.hpp"
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

    
    wali::witness::WitnessWrapper* wrapper = new wali::witness::WitnessWrapper();

    WPDS* pds = new WPDS( wrapper );
    wali_key_t p = getKey("p");
    wali_key_t acc = getKey("acc");
    wali_key_t n0 = getKey("n0");
    wali_key_t n1 = getKey("n1");
    wali_key_t n2 = getKey("n2");
    wali_key_t n3 = getKey("n3");
    // true branch
    pds->add_rule(p,n0,p,n1,reach);
    // false branch
    pds->add_rule(p,n0,p,n2,reach);
    // join point
    pds->add_rule(p,n1,p,n3,reach);
    pds->add_rule(p,n2,p,n3,reach);

    pds->print( std::cout << "---- WPDS ----\n" ) << std::endl;
    WFA fain;
    fain.set_initial_state(p);
    fain.add_final_state(acc);
    fain.add_trans(p,getKey("n0"),acc,reach);
    fain.print( cout << "----- WFA BEFORE -----\n" ) << std::endl;

    WFA faout = pds->poststar(fain);
    faout.print( cout << "----- WFA AFTER -----\n" ) << std::endl;

    delete wrapper;
    delete pds;
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

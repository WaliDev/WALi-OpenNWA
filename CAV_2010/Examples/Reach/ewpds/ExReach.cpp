/*

// Pseudo Code //
x = 0
y = 0

fun f()
    n0: <$ f enter node $>
    n1: if( x = 0 )
    n2:     then y := 1
    n3:     else y := 2
    n4: g()
    n5: <$ f exit node $>

fun g()
    n6: <$ g enter node $>
    n7: y := 4
    n8: x := 60
    n9: <$ g exit node $>

 */
#include <wali/wpds/ewpds/EWPDS.hpp>
#include <wali/wpds/ewpds/ERule.hpp>
#include <wali/wpds/Config.hpp>
#include <wali/wfa/Trans.hpp>
#include <wali/wfa/State.hpp>
#include "../Reach.hpp"
#include <string>
#include <sstream>

void doReach()
{
    using wali::wali_key_t;
    using wali::wpds::ewpds::EWPDS;
    using wali::wfa::WFA;

    Reach* reachOne = new Reach(true);
    EWPDS myEwpds;
    wali_key_t p = wali::getKey("p");
    wali_key_t accept = wali::getKey("accept");
    wali_key_t n[10];
    for( int i=0 ; i < 10 ; i++ ) {
        std::stringstream ss;
        ss << "n" << i;
        n[i] = wali::getKey( ss.str() );
    }

    // f intraprocedural
    myEwpds.add_rule( p, n[0], p, n[1], reachOne);
    myEwpds.add_rule( p, n[1], p, n[2], reachOne);
    myEwpds.add_rule( p, n[1], p, n[3], reachOne);
    myEwpds.add_rule( p, n[2], p, n[4], reachOne);
    myEwpds.add_rule( p, n[3], p, n[4], reachOne);

    // g intraprocedural
    myEwpds.add_rule( p, n[6], p, n[7], reachOne);
    myEwpds.add_rule( p, n[7], p, n[8], reachOne);
    myEwpds.add_rule( p, n[8], p, n[9], reachOne);

    // f call g
    myEwpds.add_rule( p, n[4], p, n[6], n[5], reachOne);

    // f return
    myEwpds.add_rule( p, n[5] , p , reachOne);

    // g return
    myEwpds.add_rule( p, n[9] , p , reachOne);

    // Print the EWPDS
    myEwpds.print( std::cerr ) << std::endl;

    // Perform poststar query
    WFA query;
    std::cerr << "\t> Adding p,n0,acc to query...";
    query.addTrans( p, n[0], accept, reachOne );
    std::cerr << "> done\n";
    //query.add_initial_state( p );
    //query.add_final_state( accept );
    query.print( std::cerr << "BEFORE poststar\n" ) << std::endl;
    WFA answer = myEwpds.poststar(query);
    answer.print( std::cerr << "\nAFTER poststar\n" ) << std::endl;

    // Perfor prestar query
    WFA prequery;
    prequery.addTrans( p, n[4], accept, reachOne );
    //query.add_initial_state( p );
    //query.add_final_state( accept );
    prequery.print( std::cerr << "BEFORE prestar\n" ) << std::endl;
    answer = myEwpds.prestar(prequery);
    answer.print( std::cerr << "\nAFTER prestar\n" ) << std::endl;

    /*
    // Perform path summary query
    answer.path_summary();
    ref_ptr< Reach > pWeight = answer.state_weight( p );
    std::cerr << "Weight on state \"p\": ";
    pWeight->print( std::cerr ) << std::endl;
    */


    /*
    // Perform reglang query
    wpds::CA< Reach > reglang( s );
    Reach *ignored = new Reach( true );

    //manually add transitions of the regular expression
    reglang.add( str2key("t1") , n[9] , str2key("t2") , ignored);
    reglang.add( str2key("t2") , n[5] , str2key("t3") , ignored);
    reglang.add_initial_state( str2key("t1") );
    reglang.add_final_state( str2key("t3") );

    // answer is the same CA from the earlier poststar example
    ref_ptr< Reach > reglangWeight = answer.reglang_query( reglang );
    std::cerr << "Result of reglang_query: ";
    reglangWeight->print( std::cerr ) << std::endl;
    */

}

int main()
{
    doReach();
    std::cerr << "# Trans : " << wali::wfa::Trans::numTrans << std::endl;
    std::cerr << "# States : " << wali::wfa::State::numStates << std::endl;
    std::cerr << "# Rules : " << wali::wpds::Rule::numRules << std::endl;
    std::cerr << "# Configs : " << wali::wpds::Config::numConfigs << std::endl;
    std::cerr << "# Reaches : " << Reach::numReaches << std::endl;
    return 0;
}
/*
 * $Log $
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


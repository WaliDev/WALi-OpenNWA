/**
 * @author Nick Kidd
 * @version $Id: ExReach.cpp 446 2008-07-29 08:42:49Z kidd $
 */

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
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "Reach.hpp"
#include <string>
#include <sstream>
#include <fstream>

void doReach()
{
  using wali::Key;
  using wali::wpds::WPDS;
  using wali::wfa::WFA;

  wali::sem_elem_t reachOne( new Reach(true) );
  //wali::wpds::WPDS myWpds;
  wali::wpds::fwpds::FWPDS myWpds;
  Key p = wali::getKey("p");
  Key accept = wali::getKey("accept");
  Key n[10];
  for( int i=0 ; i < 10 ; i++ ) {
    std::stringstream ss;
    ss << "n" << i;
    n[i] = wali::getKey( ss.str() );
  }

  // f intraprocedural
  myWpds.add_rule( p, n[0], p, n[1], reachOne);
  myWpds.add_rule( p, n[1], p, n[2], reachOne);
  myWpds.add_rule( p, n[1], p, n[3], reachOne);
  myWpds.add_rule( p, n[2], p, n[4], reachOne);
  myWpds.add_rule( p, n[3], p, n[4], reachOne);
  // g intraprocedural
  myWpds.add_rule( p, n[6], p, n[7], reachOne);
  myWpds.add_rule( p, n[7], p, n[8], reachOne);
  myWpds.add_rule( p, n[8], p, n[9], reachOne);
  // f call g
  myWpds.add_rule( p, n[4], p, n[6], n[5], reachOne);
  // f return
  myWpds.add_rule( p, n[5] , p , reachOne);
  // g return
  myWpds.add_rule( p, n[9] , p , reachOne);
  // Print the WPDS
  myWpds.print( std::cerr ) << std::endl;
  std::ofstream fxml( "myWpds.xml" );
  myWpds.marshall( fxml );
  fxml.close();

  // Perform poststar query
  WFA query;
  std::cerr << "\t> Adding p,n0,acc to query...";
  query.addTrans( p, n[0], accept, reachOne );
  std::cerr << "> done\n";
  query.set_initial_state( p );
  query.add_final_state( accept );
  query.print( std::cerr << "BEFORE poststar\n" ) << std::endl;
  WFA answer;
  myWpds.poststar(query,answer);
  answer.print( std::cerr << "\nAFTER poststar\n" ) << std::endl;

  // Perfor prestar query
  WFA prequery;
  prequery.addTrans( p, n[4], accept, reachOne );
  prequery.set_initial_state( p );
  prequery.add_final_state( accept );
  prequery.print( std::cerr << "BEFORE prestar\n" ) << std::endl;
  myWpds.prestar(prequery,answer);
  answer.print( std::cerr << "\nAFTER prestar\n" ) << std::endl;
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


/**
 * @author Nick Kidd
 *
 * Tests Witnesses
 */
#include <iostream>
#include <fstream>
#include "wali/Common.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"

#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessWrapper.hpp"
#include "wali/witness/VisitorDot.hpp"

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
// n3:  choose( call n0 , goto n4 )
// n4: 
//
void TestWpds(bool post)
{
    using wali::wpds::WPDS;
    using wali::getKey;
    using wali::wali_key_t;
    using wali::sem_elem_t;
    using wali::wfa::WFA;
    using std::cout;
    sem_elem_t R = new Reach(true);

    
    wali::witness::WitnessWrapper* wrapper = new wali::witness::WitnessWrapper();

    WPDS* pds = new WPDS( wrapper );
    //WPDS* pds = new WPDS( 0 );
    wali_key_t p = getKey("p");
    wali_key_t acc = getKey("acc");
    wali_key_t n0 = getKey("n0");
    wali_key_t n1 = getKey("n1");
    wali_key_t n2 = getKey("n2");
    wali_key_t n3 = getKey("n3");
    wali_key_t n4 = getKey("n4");
    // true branch
    pds->add_rule(p,n0,p,n1,R->one());
    pds->add_rule(p,n0,p,n1,R->one());
    //pds->print( std::cout << "\n---- WPDS ----\n" ) << std::endl;

    // false branch
    pds->add_rule(p,n0,p,n2,R->one());
    pds->add_rule(p,n0,p,n2,R->one());
    //pds->print( std::cout << "\n---- WPDS ----\n" ) << std::endl;

    // join point
    pds->add_rule(p,n1,p,n3,R->one());
    pds->add_rule(p,n1,p,n3,R->one());
    pds->add_rule(p,n2,p,n3,R->one());
    pds->add_rule(p,n2,p,n3,R->one());

    // Add a recursive call
    pds->add_rule(p,n3,p,n0,n4,R->one());
    pds->add_rule(p,n3,p,n4,R->one());
    pds->add_rule(p,n4,p,R->one());

    if (post)
      pds->print( std::cout << "---- WPDS ----\n" ) << std::endl;

    WFA fain;
    fain.setInitialState(p);
    fain.addFinalState(acc);
    if (post) {
      fain.addTrans(p,getKey("n0"),acc,R->one());
    }
    else {
      fain.addTrans(p,getKey("n4"),acc,R->one());
    }
    fain.print( cout << "----- WFA BEFORE -----\n" ) << std::endl;

    WFA faout;
    if (post)
      pds->poststar(fain,faout);
    else
      pds->prestar(fain,faout);

    faout.print( cout << "----- WFA AFTER -----\n" ) << std::endl;

    wali::wfa::Trans t;
    if( faout.find(p,n3,acc,t) ) {
        sem_elem_t se = t.weight();
        std::ofstream odot("twitness.dot");
        wali::witness::VisitorDot v(odot);
        wali::witness::Witness* wit = dynamic_cast<wali::witness::Witness*>(se.get_ptr());
        if( 0 != wit ) {
            wit->accept(v);
        }
        else {
            std::cerr << "[ERROR] Weight is not a Witness.";
            assert(wit);
        }
    }
    delete pds;
}

void TestFwpds(bool post) 
{
    using wali::wpds::fwpds::FWPDS;
    using wali::getKey;
    using wali::wali_key_t;
    using wali::sem_elem_t;
    using wali::wfa::WFA;
    using std::cout;
    sem_elem_t R = new Reach(true);

    
    wali::witness::WitnessWrapper* wrapper = new wali::witness::WitnessWrapper();

    FWPDS* pds = new FWPDS( wrapper );
    //WPDS* pds = new WPDS( 0 );
    wali_key_t p = getKey("p");
    wali_key_t acc = getKey("acc");
    wali_key_t n0 = getKey("n0");
    wali_key_t n1 = getKey("n1");
    wali_key_t n2 = getKey("n2");
    wali_key_t n3 = getKey("n3");
    wali_key_t n4 = getKey("n4");
    // true branch
    pds->add_rule(p,n0,p,n1,R->one());
    pds->add_rule(p,n0,p,n1,R->one());
    //pds->print( std::cout << "\n---- WPDS ----\n" ) << std::endl;

    // false branch
    pds->add_rule(p,n0,p,n2,R->one());
    pds->add_rule(p,n0,p,n2,R->one());
    //pds->print( std::cout << "\n---- WPDS ----\n" ) << std::endl;

    // join point
    pds->add_rule(p,n1,p,n3,R->one());
    pds->add_rule(p,n1,p,n3,R->one());
    pds->add_rule(p,n2,p,n3,R->one());
    pds->add_rule(p,n2,p,n3,R->one());

    // A recursive call
    pds->add_rule(p,n3,p,n0,n4,R->one());
    pds->add_rule(p,n3,p,n4,R->one());
    pds->add_rule(p,n4,p,R->one());

    if (post)
      pds->print( std::cout << "---- WPDS ----\n" ) << std::endl;
    WFA fain;
    fain.setInitialState(p);
    fain.addFinalState(acc);
    if (post) {
      fain.addTrans(p,getKey("n0"),acc,R->one());
    }
    else {
      fain.addTrans(p,getKey("n4"),acc,R->one());
    }
    fain.print( cout << "----- WFA BEFORE -----\n" ) << std::endl;

    WFA faout;
    if (post)
      pds->poststar(fain,faout);
    else
      pds->prestar(fain,faout);
    faout.print( cout << "----- WFA AFTER -----\n" ) << std::endl;

    wali::wfa::Trans t;
    if( faout.find(p,n3,acc,t) ) {
        sem_elem_t se = t.weight();
        std::ofstream odot("twitness.dot");
        wali::witness::VisitorDot v(odot);
        wali::witness::Witness* wit = dynamic_cast<wali::witness::Witness*>(se.get_ptr());
        if( 0 != wit ) {
            wit->accept(v);
        }
        else {
            std::cerr << "[ERROR] Weight is not a Witness.";
            assert(wit);
        }
    }
    delete pds;
}

int main()
{
  TestWpds(true);
  TestWpds(false);
  TestFwpds(true);
  TestFwpds(false);
  std::cerr << "# Trans     : " << wali::wfa::Trans::numTrans << std::endl;
  std::cerr << "# States    : " << wali::wfa::State::numStates << std::endl;
  std::cerr << "# Rules     : " << wali::wpds::Rule::numRules << std::endl;
  std::cerr << "# Reaches   : " << Reach::numReaches << std::endl;
  std::cerr << "# Witnesses : " << wali::witness::Witness::COUNT << std::endl;
  return 0;
}


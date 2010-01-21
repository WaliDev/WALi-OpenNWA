#include <iostream>
#include "VarSet.hpp"
#include "GenKillTransformer_T.hpp"
#include "wali/Common.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"

/*
Example program to be analyzed

int a, b, c, d;

void main() {
  a = 5;
  if (...) {
    b = 3;
  }
  else {
    b = 4;
  }
  p();           // call site: main_calls_p
  return;
}

void p() {
  if (...) {
    c = 8;
    d = 4;
    p();         // call site: p_calls_p1
  }
  else if (...) {
    c = 8;
    p();         // call site: p_calls_p2
    d = 4;
  }
  return;
}
*/

void print_output( const wali::wfa::Trans& goal ) {
    goal.weight()->print( std::cout );
    std::cout << std::endl << std::endl;
}

int main()
{
    using std::cout;
    using std::endl;
    wali::wpds::fwpds::FWPDS pds;
    wali::wpds::WPDS readout;
    std::set< wali::Key > R;
    std::set< wali::Key > B;

#ifdef TRACE
    traceFile = stderr;
#endif
#ifdef REFTRACE
    refTraceFile = stderr;
#endif

    /* PDS States */
    wali::Key q = wali::getKey("q");

    /* Nodes for procedure main */

    wali::Key e_main = wali::getKey("e_main");B.insert(e_main);
    wali::Key n1 = wali::getKey("n1");B.insert(n1);
    wali::Key n2 = wali::getKey("n2");B.insert(n2);
    wali::Key n3 = wali::getKey("n3");B.insert(n3);
    wali::Key n4 = wali::getKey("n4");B.insert(n4);
    wali::Key n5 = wali::getKey("n5");B.insert(n5);
    wali::Key n6 = wali::getKey("n6");B.insert(n6);
    wali::Key x_main = wali::getKey("x_main");B.insert(x_main);
    /* Nodes for procedure p */
    wali::Key e_p = wali::getKey("e_p");B.insert(e_p);
    wali::Key n7 = wali::getKey("n7");B.insert(n7);
    wali::Key n8 = wali::getKey("n8");B.insert(n8);
    wali::Key n9 = wali::getKey("n9");B.insert(n9);
    wali::Key n10 = wali::getKey("n10");B.insert(n10);
    wali::Key n11 = wali::getKey("n11");B.insert(n11);
    wali::Key n12 = wali::getKey("n12");B.insert(n12);
    wali::Key n13 = wali::getKey("n13");B.insert(n13);
    wali::Key n14 = wali::getKey("n14");B.insert(n14);
    wali::Key n15 = wali::getKey("n15");B.insert(n15);
    //wali::Key n16 = wali::getKey("n16");B.insert(n16);
    wali::Key x_p = wali::getKey("x_p");B.insert(x_p);

    /* Transitions (edges) for procedure main */
    pds.add_rule( q,     e_main,    q,    n1,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(VarSet::EmptySet(), mkVarSet("a","b","c","d")));
    pds.add_rule( q,     n1,        q,    n2,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), VarSet::EmptySet()));

    pds.add_rule( q,     n2,        q,    n3,     GenKillTransformer_T< VarSet >::id());
    pds.add_rule( q,     n3,        q,    n5,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("b"), VarSet::EmptySet()));

    pds.add_rule( q,     n2,        q,    n4,     GenKillTransformer_T< VarSet >::id());
    pds.add_rule( q,     n4,        q,    n5,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("b"), VarSet::EmptySet()));

    pds.add_rule( q,     n6,        q,    x_main, GenKillTransformer_T< VarSet >::id());

    /* Transitions (edges) for procedure p */
    pds.add_rule( q,     e_p,       q,    n7,     GenKillTransformer_T< VarSet >::id());

    /* if (...) */
    pds.add_rule( q,     n7,        q,    n8,     GenKillTransformer_T< VarSet >::id());
    pds.add_rule( q,     n8,        q,    n9,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("c"), VarSet::EmptySet()));
    pds.add_rule( q,     n9,        q,    n10,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("d"), VarSet::EmptySet()));
    pds.add_rule( q,     n11,       q,    x_p,    GenKillTransformer_T< VarSet >::id());

    /* else if (...) */
    pds.add_rule( q,     n7,        q,    n12,    GenKillTransformer_T< VarSet >::id());
    pds.add_rule( q,     n12,       q,    n13,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("c"), VarSet::EmptySet()));
    pds.add_rule( q,     n14,       q,    n15,    GenKillTransformer_T< VarSet >::id());
    pds.add_rule( q,     n15,       q,    x_p,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("d"), VarSet::EmptySet()));

    /* else */
    pds.add_rule( q,     n7,        q,    x_p,    GenKillTransformer_T< VarSet >::id());

    /* Transitions (linkage edges) for main-calls-p and return */
    pds.add_rule( q,     n5,        q,    e_p,   n6, GenKillTransformer_T< VarSet >::id());

    /* Transitions (linkage edges) for p-calls-p1 and return */
    pds.add_rule( q,     n10,       q,    e_p,  n11, GenKillTransformer_T< VarSet >::id());

    /* Transitions (linkage edges) for p-calls-p2 and return */
    pds.add_rule( q,     n13,      q,     e_p,  n14, GenKillTransformer_T< VarSet >::id());

    /* Transitions (linkage edges) for returning from p */
    pds.add_rule( q,     x_p,       q,     GenKillTransformer_T< VarSet >::id());

    /* PDS transitions for "consuming" (backwards) the call graph to enclosing procedure */
    /* (one set for each CFG node)                                                       */
    wali::Key accepting_state = wali::getKey("accepting_state");

    /* debug print wpds */
    pds.print( std::cout ) << "\n----------------------------------------\n";
    /* pre* o post* of <q, e_main> */
    printf("pre* o post* of <q, e_main>\n\n");
    wali::wfa::WFA ca1;
    ca1.addTrans( q, e_main, accepting_state, GenKillTransformer_T< VarSet >::id());
    ca1.addFinalState( accepting_state );
    ca1.setInitialState( q );
    ca1.print( std::cout << "before\n" ) << std::endl;
    wali::wfa::WFA ca3;
    pds.poststar(ca1,ca3);
    ca3.print( std::cout << "after\n" ) << std::endl;

    /* PDS transitions for "consuming" the language (n11 + n14 + n6)*  */
    R.insert( n6 );
    R.insert( n11 );
    R.insert( n14 );
    R.insert( e_main );
    std::cout << "\n\n--- Calling path_summary ---\n";
    //ca3.path_summary( R );
    ca3.path_summary();
#if 0
    std::cout << "--- Done path_summary ---\n";

    // for each b element of B, print (q,b,q') and its weight
    for(std::set< wali::Key >::iterator iter = B.begin();
            iter != B.end(); iter++ ) {
        wali::Key b = *iter;
        //wali::Key b = n18;
        wpds::CA< GenKillTransformer_T< VarSet > >::TransSetIterPair pair = ca3.match( q,b );
        wpds::CA< GenKillTransformer_T< VarSet > >::TransSetIter titer = pair.first;
        wpds::Semiring< GenKillTransformer_T< VarSet > >::SemiringElement V_b(s.zero());
        //std::cout << "BEGINNING " << key2str(b) << "---------------\n";
        for( ; titer != pair.second; titer++ ) {
            wpds::CA< GenKillTransformer_T< VarSet > >::catrans_t t( *titer );

            //std::cout << "extending:\n\t" << ca3.state_weight(t->to_state()) << "\n\t" << t->semiring_element() << std::endl;

            wpds::Semiring< GenKillTransformer_T< VarSet > >::SemiringElement tmp(
                    s.extend( ca3.state_weight(t->to_state()),t->semiring_element() ) );

            //std::cout << "combining:\n\t" << V_b << "\n\t" << tmp << std::endl;
            V_b = s.combine(V_b,tmp);
        }
                printkey(b, std::cout) << ": " << V_b << "\n\n";
        //std::cout << "ENDING " << key2str(b) << "---------------\n\n";
    }
#endif
#if 1
    //std::cout << "middle\n" << ca3 << std::endl;

    //ca1 = pds.prestar(ca1);
    //ca1.print( std::cout << "after\n" ) << std::endl;

    wali::wfa::Trans goal;
    if( ca3.find(q,e_main,accepting_state,goal) ) {    cout << "e_main: ";print_output(goal);}
    if( ca3.find(q,n1,accepting_state,goal) ) {    cout << "n1: ";print_output(goal);}
    if( ca3.find(q,n2,accepting_state,goal) ) {    cout << "n2: ";print_output(goal);}
    if( ca3.find(q,n3,accepting_state,goal) ) {    cout << "n3: ";print_output(goal);}
    if( ca3.find(q,n4,accepting_state,goal) ) {    cout << "n4: ";print_output(goal);}
    if( ca3.find(q,n5,accepting_state,goal) ) {    cout << "n5: ";print_output(goal);}
    if( ca3.find(q,n6,accepting_state,goal) ) {    cout << "n6: ";print_output(goal);}
    if( ca3.find(q,x_main,accepting_state,goal) ) {    cout << "x_main: ";print_output(goal);}

    if( ca3.find(q,e_p,accepting_state,goal) ) {    cout << "e_p: ";print_output(goal);}
    if( ca3.find(q,n7,accepting_state,goal) ) {    cout << "n7: ";print_output(goal);}
    if( ca3.find(q,n8,accepting_state,goal) ) {    cout << "n8: ";print_output(goal);}
    if( ca3.find(q,n9,accepting_state,goal) ) {    cout << "n9: ";print_output(goal);}
    if( ca3.find(q,n10,accepting_state,goal) ) {    cout << "n10: ";print_output(goal);}
    if( ca3.find(q,n11,accepting_state,goal) ) {    cout << "n11: ";print_output(goal);}
    if( ca3.find(q,n12,accepting_state,goal) ) {    cout << "n12: ";print_output(goal);}
    if( ca3.find(q,n13,accepting_state,goal) ) {    cout << "n13: ";print_output(goal);}
    if( ca3.find(q,n14,accepting_state,goal) ) {    cout << "n14: ";print_output(goal);}
    if( ca3.find(q,n15,accepting_state,goal) ) {    cout << "n15: ";print_output(goal);}
    if( ca3.find(q,x_p,accepting_state,goal) ) {    cout << "x_p: ";print_output(goal);}
#endif

    return 0;
} /** end of main **/

#include <iostream>
#include <string>

#include "VarSet.hpp"
#include "GenKillTransformer_T.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wfa/WFA.hpp"

/*
Example program to be analyzed

int a, b, c, d;

void main() {
  a = 1;
  b = 2;
  if (...) {
    c = 3;
  }
  else {
    a = 4;
  }
  p();           // call site: main_calls_p
  c = d + b;
  return;
}

void p() {
  if (...) {
    a = c;
    a = 10;
    p();         // call site: p_calls_p1
    d = c
  }
  else if (...) {
    b = 9;
    a = a + d;
    p();         // call site: p_calls_p2
    d = b;
  }
  return;
}
*/

void print_output( const wali::wfa::Trans& goal )
{
    goal.weight()->print( std::cout );
    std::cout << std::endl << std::endl;
}

int main() {

    //wali::wpds::WPDS wpds;
    wali::wpds::fwpds::FWPDS wpds;
    wali::wfa::WFA ca0;
    std::set< wali::Key > R;
    std::set< wali::Key > B;


    //std::cout << "s.diff(one(),one()) : " << s.diff(s.one(),s.one()) << std::endl;

    /* Variables */
    wali::Key q = wali::getKey("q");

    /* Nodes for procedure main */

    wali::Key e_main = wali::getKey("e_main");B.insert(e_main);
    wali::Key n1 = wali::getKey("n1");B.insert(n1);
    wali::Key n2 = wali::getKey("n2");B.insert(n2);
    wali::Key n3 = wali::getKey("n3");B.insert(n3);
    wali::Key n4 = wali::getKey("n4");B.insert(n4);
    wali::Key n5 = wali::getKey("n5");B.insert(n5);
    wali::Key n6 = wali::getKey("n6");B.insert(n6);
    wali::Key n7 = wali::getKey("n7");B.insert(n7);
    wali::Key n8 = wali::getKey("n8");B.insert(n8);
    wali::Key x_main = wali::getKey("x_main");B.insert(x_main);

    /* Nodes for procedure p */
    wali::Key e_p = wali::getKey("e_p");B.insert(e_p);
    wali::Key n9 = wali::getKey("n9");B.insert(n9);
    wali::Key n10 = wali::getKey("n10");B.insert(n10);
    wali::Key n11 = wali::getKey("n11");B.insert(n11);
    wali::Key n12 = wali::getKey("n12");B.insert(n12);
    wali::Key n13 = wali::getKey("n13");B.insert(n13);
    wali::Key n14 = wali::getKey("n14");B.insert(n14);
    wali::Key n15 = wali::getKey("n15");B.insert(n15);
    wali::Key n16 = wali::getKey("n16");B.insert(n16);
    wali::Key n17 = wali::getKey("n17");B.insert(n17);
    wali::Key n18 = wali::getKey("n18");B.insert(n18);
    wali::Key n19 = wali::getKey("n19");B.insert(n19);
    wali::Key x_p = wali::getKey("x_p");B.insert(x_p);

    /* States for a configuration automaton */
    wali::Key accepting_state = wali::getKey("accepting_state");

    /* Transition to introduce stack bottom */
    wali::Key stack_bottom = wali::getKey("stack_bottom");

    wpds.add_rule(q,stack_bottom,q,x_main,stack_bottom,GenKillTransformer_T< VarSet >::id());

    /* Transitions (edges) for procedure main */
    wpds.add_rule(q,    x_main, q,    n8,     GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,    n8, q,    n7,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("c"), mkVarSet("d","b")));

    wpds.add_rule(q,     n6,   q,    n4,     GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n4,   q,    n3,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("c"), VarSet::EmptySet()));

    wpds.add_rule(q,     n6,   q,    n5,     GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n5,   q,    n3,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), VarSet::EmptySet()));

    wpds.add_rule(q,     n3,   q,    n2,     GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n2,   q,    n1,     GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("b"), VarSet::EmptySet()));
    wpds.add_rule(q,     n1,   q,    e_main, GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), VarSet::EmptySet()));

    /* Transitions (edges) for procedure p */

    // Simple return path
    wpds.add_rule(q,     x_p,  q,     n9,    GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n9,   q,    e_p,    GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     e_p,       q,     GenKillTransformer_T< VarSet >::id());

    wpds.add_rule(q,     x_p,  q,    n14,    GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n14,  q,    n13,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("d"), mkVarSet("c")));

    wpds.add_rule(q,     x_p,  q,    n19,    GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n19,  q,    n18,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("d"), mkVarSet("b")));

    wpds.add_rule(q,     n12,  q,    n11,    GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n11,  q,    n10,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), VarSet::EmptySet()));
    wpds.add_rule(q,     n10,  q,     n9,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), mkVarSet("c")));

    wpds.add_rule(q,     n17,  q,    n16,    GenKillTransformer_T< VarSet >::id());
    wpds.add_rule(q,     n16,  q,    n15,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), mkVarSet("a","d")));
    wpds.add_rule(q,     n15,  q,     n9,    GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("b"), VarSet::EmptySet()));


    /* Transitions (linkage edges) for main-calls-p and return */
    /******************* FOR DEBUGGING, REMOVE call to P() *********************/
    wpds.add_rule(q,     n7,        q,    x_p,   n6, GenKillTransformer_T< VarSet >::id());
    //wpds.add_rule(q,  n7,         q,      n6,     GenKillTransformer_T< VarSet >::id());
    /******************* FOR DEBUGGING, REMOVE call to P() *********************/

    /* Transitions (linkage edges) for p-calls-p1 and return */
    wpds.add_rule(q,     n13,       q,    x_p,  n12, GenKillTransformer_T< VarSet >::id());
    //wpds.add_rule(q,     n13,       q,    n12, GenKillTransformer_T< VarSet >::id());

    /* Transitions (linkage edges) for p-calls-p2 and return */
    wpds.add_rule(q,     n18,      q,     x_p,  n17, GenKillTransformer_T< VarSet >::id());
    //wpds.add_rule(q,     n18,      q,     n17, GenKillTransformer_T< VarSet >::id());



    /* post* of <q, stack_bottom> */

    wpds.print( std::cout << "WPDS... \n" ) << "\n-------------------------\n";
    printf("post* of <q, stack_bottom>\n\n");
    wali::wfa::WFA ca2;
    ca2.addTrans(q, stack_bottom, accepting_state, GenKillTransformer_T< VarSet >::id());
    ca2.addFinalState( accepting_state );
    ca2.setInitialState( q );
    ca2.print( std::cout << "Before ...\n" ) << std::endl;

    wali::wfa::WFA ca3;
    wpds.poststar(ca2,ca3);
    ca3.print( std::cout << "After ...\n" ) << std::endl;

    /* PDS transitions for "consuming" the language (n12 + n17 + n6)*  */
    R.insert( n6 );
    R.insert( n12 );
    R.insert( n17 );
    R.insert( stack_bottom );
    std::cout << "\n\n--- Calling path_summary ---\n";

    ca3.path_summary();
    std::cout << "--- Done path_summary ---\n";
    
    return 0;

} /** end of main **/

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

#include "wali/wpds/WPDS.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/WeightMaker.hpp"
#include "Must.hpp"
#include <iostream>

/*

void f() {
f1:     char buf[] = "hello\n";
    if( ? )
        f()
}

int main()
{
n1:     f();
n2:     return 0;
}

*/

int main()
{
    using namespace wali;
    sem_elem_t tmp( new Must(0,0) );

    wfa::WFA S;
    S.addTrans( getKey("p"), getKey("n2"), getKey("Sacc"), tmp->one() );
    S.setInitialState( getKey("p") );
    S.addFinalState( getKey("Sacc") );

    wfa::WFA T;
    T.addTrans( getKey("p"), getKey("n0"), getKey("Tacc"), tmp->one() );
    T.setInitialState( getKey("p") );
    T.addFinalState( getKey("Tacc") );

    wpds::WPDS pds;
    // choice, call f or not
    pds.add_rule( getKey("p"), getKey("n0"), getKey("p"), getKey("n1"), tmp->one() );
    pds.add_rule( getKey("p"), getKey("n0"), getKey("p"), getKey("n2"), tmp->one() );
    // call f
    pds.add_rule( getKey("p"), getKey("n1"), getKey("p"), getKey("f_enter"), getKey("n2"), tmp->one() );
    // ret main
    pds.add_rule( getKey("p"), getKey("n2"), getKey("p"), tmp->one() );

    // f
    pds.add_rule( getKey("p"), getKey("f_enter"), getKey("p"), getKey("f1"), new Must(16,16) );
    pds.add_rule( getKey("p"), getKey("f1"), getKey("p"), getKey("f2"), tmp->one() );
    pds.add_rule( getKey("p"), getKey("f2"), getKey("p"), getKey("f_enter"), getKey("f_exit"), tmp->one() );
    pds.add_rule( getKey("p"), getKey("f1"), getKey("p"), getKey("f_exit"), tmp->one() );
    pds.add_rule( getKey("p"), getKey("f_exit"), getKey("p"), new Must(-16,0) );

    pds.print( std::cout << "+++ The PDS +++\n" ) << std::endl;

    wfa::WFA preS = pds.prestar( S );
    preS.print( std::cout << "+++ Pre* from S +++\n" ) << std::endl;

    wfa::WFA postT = pds.poststar( T );
    postT.print( std::cout << "+++ Post* from T +++\n" ) << std::endl;

    wfa::KeepRight wmaker;
    wfa::WFA result = postT.intersect( wmaker, preS );

    result.prune();

    result.print( std::cout << "+++ post*(T) \\cap pre*(S) +++\n" ) << std::endl;

    return 0;
}

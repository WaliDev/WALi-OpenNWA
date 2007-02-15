/*!
 * @author Nick Kidd
 */

#include "VarSet.hpp"
#include "GenKillTransformer_T.hpp"

#include "wali/Common.hpp"


#include "wali/wfa/WFA.hpp"

#include "wali/witness/WitnessWrapper.hpp"

#include "wali/wpds/WPDS.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"

using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::fwpds;

int main()
{
    sem_elem_t c(GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("c"), mkVarSet("c")));
    sem_elem_t a(GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("a"), mkVarSet("a")));
    sem_elem_t b(GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("b"), mkVarSet("b")));
    sem_elem_t d(GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("d"), mkVarSet("d")));
    sem_elem_t e(GenKillTransformer_T< VarSet >::makeGenKillTransformer_T(mkVarSet("e"), mkVarSet("e")));

    FWPDS pds( new wali::witness::WitnessWrapper() );
    pds.add_rule(getKey("p"),getKey("n0"),getKey("p"),getKey("n2"),a);
    pds.add_rule(getKey("p"),getKey("n0"),getKey("p"),getKey("n1"),GenKillTransformer_T< VarSet >::id());
    //pds.add_rule(getKey("p"),getKey("n0"),getKey("p"),getKey("n1"),b);
    pds.add_rule(getKey("p"),getKey("n1"),getKey("p"),getKey("n0"),getKey("n3"),c);
    pds.add_rule(getKey("p"),getKey("n2"),getKey("p"),getKey("n3"),GenKillTransformer_T< VarSet>::id());
    //pds.add_rule(getKey("p"),getKey("n2"),getKey("p"),getKey("n3"),d);
    pds.add_rule(getKey("p"),getKey("n3"),getKey("p"), GenKillTransformer_T< VarSet >::id());

    WFA in;
    in.setInitialState(getKey("p"));
    in.addFinalState(getKey("acc"));
	 in.addTrans(getKey("p"),getKey("n0"),getKey("acc"),GenKillTransformer_T< VarSet >::id());
    //in.addTrans(getKey("p"),getKey("n0"),getKey("acc"),e );

    WFA out;
    pds.poststar(in,out);

    out.print( std::cout ) << std::endl;

    return 0;
}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/


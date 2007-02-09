/*!
 * @author Nick Kidd
 */

#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/Worklist.hpp"
#include "wali/Common.hpp"
#include "wali/HashMap.hpp"
#include "wali/util/Timer.hpp"
#include "wali/SemElem.hpp"

using namespace wali::wpds::fwpds;

FWPDS::FWPDS() : WPDS()
{
}

FWPDS::FWPDS(Wrapper* wrapper) : WPDS(wrapper) 
{
}

FWPDS::FWPDS( Worklist<wfa::Trans> * worklist ) : WPDS(worklist)
{
}

FWPDS::FWPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist ) :
    WPDS(wrapper,worklist)
{
}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/


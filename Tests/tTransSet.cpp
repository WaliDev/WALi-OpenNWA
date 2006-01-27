/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/WFA.hpp"
#include "Reach.hpp"

int main()
{
    using wali::Key;
    using wali::getKey;
    using wali::wfa::Trans;
    using wali::wfa::TransSet;
    using wali::wfa::WFA;
    Key a = getKey("a");
    Key b = getKey("b");
    Key c = getKey("c");
    Key d = getKey("d");

    TransSet s;

    Reach * ONE = new Reach(true);
    s.insert( new Trans(a,b,c,ONE) );
    s.insert( new Trans(a,getKey("b"),d,ONE) );
    s.print( std::cout << "TransSet " ) << std::endl;

    WFA fa;
    fa.addTrans( new Trans(a,b,c,ONE) );
    fa.addTrans( new Trans(a,getKey("b"),d,ONE) );
    fa.print( std::cout ) << std::endl;

    return 0;


}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

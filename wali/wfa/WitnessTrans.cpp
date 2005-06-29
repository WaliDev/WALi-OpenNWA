/*!
 * @author Nick Kidd
 * @version $Revision: 1.4 $
 */

#include "wali/wfa/WitnessTrans.hpp"

namespace wali
{
    namespace wfa
    {
        WitnessTrans::WitnessTrans( const Trans& t_ ) : Witness(t_.weight()),t(t_)
        {
        }

        // Destructor does nothing.
        WitnessTrans::~WitnessTrans()
        {
        }

        // Overrides Witness::pretty_print
        std::ostream& WitnessTrans::pretty_print( std::ostream& o,size_t depth ) const
        {
            format_depth(o,depth);
            o << "WitnessTrans: ";
            t.print(o) << std::endl;
            return o;
        }

        const Trans& WitnessTrans::trans() const
        {
            return t;
        }
            
    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

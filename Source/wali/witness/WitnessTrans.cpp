/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/Visitor.hpp"

namespace wali
{
    using wfa::Trans;

    namespace witness
    {
        WitnessTrans::WitnessTrans( const Trans& t_ ) : Witness(t_.weight()),t(t_)
        {
        }

        // Destructor does nothing.
        WitnessTrans::~WitnessTrans()
        {
        }

        //
        // Override Witness::accept
        //
        void WitnessTrans::accept( Visitor& v )
        {
            v.visitTrans(this);
        }

        // Overrides Witness::prettyPrint
        std::ostream& WitnessTrans::prettyPrint( std::ostream& o,size_t depth ) const
        {
            formatDepth(o,depth);
            o << "WitnessTrans: ";
            t.print(o) << std::endl;
            return o;
        }

        const Trans& WitnessTrans::getTrans() const
        {
            return t;
        }
            
        sem_elem_t WitnessTrans::extend( SemElem * se )
        {
            //print( std::cout << "\n\nWitnessTrans::Extend --\n\tThis: ");
            //se->print( std::cout << "\n\tThat: ") << std::endl;
            //assert(0);
            return Witness::extend(se);
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

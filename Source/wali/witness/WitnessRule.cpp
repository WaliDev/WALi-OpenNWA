/*!
 * @author Nick Kidd
 */
#include "wali/Common.hpp"
#include "wali/witness/WitnessRule.hpp"

namespace wali
{
    using wpds::Rule;

    namespace witness
    {
        WitnessRule::WitnessRule( const Rule& r_t ) :
            Witness(r_t.weight()),
            // TODO :
            //      WitnessRule's need their own Configs if
            //      their scope is to last beyond that of the
            //      WPDS that created it.
            // 
            //fconfig(r_t->from_state(),r_t->from_stack()),
            //tconfig(r_t->to_state(),r_t->to_stack1()),
            //r(&fconfig,&tconfig,r_t->to_stack2,r_t->weight())
            r(r_t)
        {
        }

        //! Destructor does nothing.
        WitnessRule::~WitnessRule()
        {
        }

        // override Witness::pretty_print
        std::ostream& WitnessRule::pretty_print( std::ostream& o,size_t depth ) const
        {
            format_depth(o,depth);
            o << "WitnessRule: ";
            r.print(o) << std::endl;
            return o;
        }

        const Rule& WitnessRule::rule() const
        {
            return r;
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

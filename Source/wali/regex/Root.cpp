/*!
 * @author Nick Kidd
 */

#include "wali/regex/Root.hpp"

namespace wali {

    namespace regex {

        Root::Root(const std::string& label) : Regex(), lbl(label)
        {
            assert(lbl.size() <= 1);
        }

        Root::Root(const std::string& label, wali::sem_elem_t se) : Regex(se),lbl(label)
        {
            assert(lbl.size() <= 1);
        }

        Root::~Root() {
        }

        // Override Regex 
        bool Root::isConstant() const 
        { 
            return true; 
        }

        std::ostream& Root::print_recurse( std::ostream& o ) const {
            o << lbl;
            return o;
        }

        void Root::to_mona_recurse( 
                std::ostream& o, const std::string& prefix ) const 
        {
            if( !marked() ) {
                mark();
                o << "pred " << prefix << "_" << id << "(var1 p, var1 q) = is_" << lbl << "(p,q);\n\n";
            }
            //assert(0);
        }

        std::ostream& Root::write_dot_recurse( std::ostream& o ) const
        {
            write_dot_state(o,lbl,"blue");
            return o;
        }

        wali::sem_elem_t Root::solve_recurse()
        {
            return value;
        }

    } // namespace regex

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */


/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessMergeFn.hpp"
#include "wali/witness/Visitor.hpp"

namespace wali
{
    namespace witness
    {
        // weight == merge->apply_f(left,right)
        WitnessMerge::WitnessMerge( 
                sem_elem_t weight, 
                witness_merge_fn_t witness_merge_fn,
                witness_t left, 
                witness_t right ) :
            Witness(weight), witness_merge_fn(witness_merge_fn),lchild(left),rchild(right) 
        {
        }

        // virtual destructor
        WitnessMerge::~WitnessMerge() {}

        //
        // Override Witness::accept
        //
        void WitnessMerge::accept( Visitor& v, bool visitOnce )
        {
            if( !marked() || !visitOnce) {
                mark();
                if( v.visitMerge(this) ) {
                    if( hasLeft() )
                        left()->accept(v,visitOnce);
                    if( hasRight() )
                        right()->accept(v,visitOnce);
                }
            }
        }

        // Override Witness::prettyPrint
        std::ostream& WitnessMerge::prettyPrint( std::ostream& o, size_t depth ) const
        {
            formatDepth(o,depth);
            o << "WitnessMerge < ";
            witness_merge_fn->print(o) << " > : ";
            user_se->print(o) << std::endl;
            // Dumps out to much information.
            // TODO. Make a compile or runtime flag.
            if( 1 ) {
                if( hasLeft() )
                {
                    lchild->prettyPrint(o,depth+1);
                    if( hasRight() )
                        rchild->prettyPrint(o,depth+1);
                }
                else {
                    // TODO : make debug
                    assert( !hasRight() );
                }
            }
            else {
                int cnt = 0;
                if( hasLeft() ) cnt++;
                if( hasRight() ) cnt++;
                o << "\tHas " << cnt << " children.\n";
            }
            return o;
        }

        void WitnessMerge::reset_marks() const
        {
            if( marked() ) {
                unmark();
                if( hasLeft() ) lchild->reset_marks();
                if( hasRight() ) rchild->reset_marks();
            }
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
 */

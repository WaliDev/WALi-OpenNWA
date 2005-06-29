/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessExtend.hpp"

namespace wali
{
    namespace witness
    {
        // weight == came from where?
        WitnessExtend::WitnessExtend( sem_elem_t weight ) :
            Witness(weight),lchild(0),rchild(0) {}

        // weight == left
        WitnessExtend::WitnessExtend( sem_elem_t weight, witness_t left ) :
            Witness(weight),lchild(left),rchild(0) {}

        // weight == left->extend( right )
        WitnessExtend::WitnessExtend( sem_elem_t weight, witness_t left, witness_t right ) :
            Witness(weight),lchild(left),rchild(right) {}

        // virtual destructor
        WitnessExtend::~WitnessExtend() {}

        // Override Witness::pretty_print
        std::ostream& WitnessExtend::pretty_print( std::ostream& o, size_t depth ) const
        {
            format_depth(o,depth);
            o << "WitnessExtend: ";
            user_se->print(o) << std::endl;
            if( has_left() )
            {
                lchild->pretty_print(o,depth+1);
                if( has_right() )
                    rchild->pretty_print(o,depth+1);
            }
            else {
                // TODO : make debug
                assert( !has_right() );
            }
            return o;
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
 */

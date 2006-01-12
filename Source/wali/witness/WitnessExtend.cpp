/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/Visitor.hpp"

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

        //
        // Override Witness::accept
        //
        void WitnessExtend::accept( Visitor& v )
        {
            v.visitExtend(this);
        }

        // Override Witness::prettyPrint
        std::ostream& WitnessExtend::prettyPrint( std::ostream& o, size_t depth ) const
        {
            formatDepth(o,depth);
            o << "WitnessExtend: ";
            user_se->print(o) << std::endl;
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
            return o;
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
 */

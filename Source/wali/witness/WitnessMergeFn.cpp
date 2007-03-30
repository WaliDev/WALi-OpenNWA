/*!
 * @author Nick Kidd
 */

#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessMergeFn.hpp"

namespace wali {

    namespace witness {

        // TODO: Do we need to pass a WitnessMerge down?
        WitnessMergeFn::WitnessMergeFn( merge_fn_t user_merge )
            : wpds::ewpds::MergeFn(/*WitnessMerge?*/),user_merge(user_merge) 
        {
        }

        WitnessMergeFn::~WitnessMergeFn() 
        {
        }

        SemElem * WitnessMergeFn::apply_f( SemElem* w1, SemElem *w2)
        {
            return priv_do_apply(w1,w2);
        }

        sem_elem_t WitnessMergeFn::apply_f( sem_elem_t a, sem_elem_t b)
        {
            SemElem* w1 = a.get_ptr();
            SemElem* w2 = b.get_ptr();
            return priv_do_apply(w1,w2);
        }

        std::ostream& WitnessMergeFn::print( std::ostream& o ) const
        {
            o << "WitnessMergeFn[ ";
            user_merge->print(o) << "]";
            return o;
        }

        SemElem* WitnessMergeFn::priv_do_apply( SemElem* w1,SemElem* w2 )
        {
            Witness* left = dynamic_cast< Witness* >(w1);
            Witness* right = dynamic_cast< Witness* >(w2);
            if( left == 0 ) {
                *waliErr << "[ERROR] Attempt to apply WitnessMergeFn to non witness.\n";
                assert(0);
            }
            if( right == 0 ) {
                *waliErr << "[ERROR] Attempt to apply WitnessMergeFn to non witness.\n";
                assert(0);
            }

            sem_elem_t user_se = user_merge->apply_f( left->weight(), right->weight());

            WitnessMerge* witmerge = new WitnessMerge(user_se,this,left,right);
            return witmerge;
        }

    } // namespace witness

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

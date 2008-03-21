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

    sem_elem_t WitnessMergeFn::apply_f( sem_elem_t a, sem_elem_t b)
    {
      return priv_do_apply(a,b);
    }

    std::ostream& WitnessMergeFn::print( std::ostream& o ) const
    {
      o << "WitnessMergeFn[ ";
      user_merge->print(o) << "]";
      return o;
    }

    sem_elem_t WitnessMergeFn::priv_do_apply( sem_elem_t a,sem_elem_t b )
    {
      Witness* left = dynamic_cast< Witness* >(a.get_ptr());
      Witness* right = dynamic_cast< Witness* >(b.get_ptr());
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


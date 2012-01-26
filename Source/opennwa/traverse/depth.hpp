#ifndef WALI_NWA_TRAVERSE_DEPTH_HPP
#define WALI_NWA_TRAVERSE_DEPTH_HPP

#include "opennwa/traverse/StateFunctor.hpp"
#include "opennwa/traverse/TransitionFunctor.hpp"

namespace wali {
  namespace nwa {
    namespace traverse {

      void dfsCallStringTraversal(NWA const & nwa,
                                  ConstCallStringStateFunctor * state_functor,
                                  ConstCallStringTransitionFunctor * trans_functor);
        
    }
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

#ifndef WALI_NWA_TRAVERSE_DEPTH_HPP
#define WALI_NWA_TRAVERSE_DEPTH_HPP

#include "wali/nwa/traverse/StateFunctor.hpp"

namespace wali {
  namespace nwa {
    namespace traverse {

      void dfsCallStringTraversal(NWA const & nwa, ConstCallStringStateFunctor & functor);
        
    }
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

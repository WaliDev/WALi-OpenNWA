#ifndef WALI_NWA_STATEFUNCTOR_HPP
#define WALI_NWA_STATEFUNCTOR_HPP

#include <vector>

#include "opennwa/NWAFwd.hpp"

namespace opennwa {
    namespace traverse {

      class ConstStateFunctor
      {
      public:
        virtual ~ConstStateFunctor() {}

        virtual void operator() (NWA const & nwa, State state) = 0;
      };

      
      class ConstCallStringStateFunctor
      {
      public:
        ///
        /// Index [0] is the bottom item on the stack. For main, the call
        /// stack will have one item in it with an unspecified value
        typedef std::vector<State> CallString;
        
        virtual ~ConstCallStringStateFunctor() {}

        virtual void operator() (NWA const & nwa, CallString const & cs, State state) = 0;
      };
      
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

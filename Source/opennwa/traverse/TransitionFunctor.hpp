#ifndef WALI_NWA_TRAVERSE_TRANSITION_FUNCTOR_HPP
#define WALI_NWA_TRAVERSE_TRANSITION_FUNCTOR_HPP

#include <vector>

#include "opennwa/NwaFwd.hpp"

namespace opennwa {
  namespace traverse {

    class ConstTransitionFunctor
    {
    public:
      virtual ~ConstTransitionFunctor() {}

      virtual void doInternal(Nwa const & nwa, State source, Symbol symbol, State target) = 0;
      virtual void doCall(Nwa const & nwa, State source, Symbol symbol, State target) = 0;
      virtual void doReturn(Nwa const & nwa, State source, State pred, Symbol symbol, State target) = 0;
    };


    class ConstCallStringTransitionFunctor
    {
    public:
      ///
      /// Index [0] is the bottom item on the stack. For main, the call
      /// stack will have one item in it with an unspecified value
      typedef std::vector<State> CallString;

      virtual ~ConstCallStringTransitionFunctor() {}

      virtual void doInternal(Nwa const & nwa, CallString const & cs,
                              State source, Symbol symbol, State target) = 0;
      virtual void doCall(Nwa const & nwa, CallString const & cs,
                          State source, Symbol symbol, State target) = 0;
      virtual void doReturn(Nwa const & nwa, CallString const & cs,
                            State source, State pred, Symbol symbol, State target) = 0;
    };
      
  }
}



// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

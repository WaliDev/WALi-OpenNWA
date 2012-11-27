#include "TransitionInfo.hpp"

// Just here for compilation. MSVC warns about an empty object file, so...

namespace opennwa {
  namespace details {
    int dummy_variable_to_avoid_msvc_warning;

    TransitionInfo::Internals const &
    TransitionInfo::emptyInternals()
    {
      static Internals r;
      return r;
    }
    
    TransitionInfo::Calls const &
    TransitionInfo::emptyCalls()
    {
      static Calls c;
      return c;
    }
    
    TransitionInfo::Returns const &
    TransitionInfo::emptyReturns()
    {
      static Returns r;
      return r;
    }
      
  }
}

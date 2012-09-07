#include "opennwa/Nwa.hpp"
#include "opennwa/query/automaton.hpp"

#include <set>

namespace opennwa {
  namespace query {

    using wali::Key;

    size_t numCallSites(Nwa const & nwa)
    {
      std::set<Key> sites;

      for(Nwa::CallIterator call = nwa.beginCallTrans();
          call != nwa.endCallTrans(); ++call)
      {
        sites.insert(call->first);
      }

      return sites.size();
    }
    
    size_t numEntrySites(Nwa const & nwa)
    {
      std::set<Key> sites;

      for(Nwa::CallIterator call = nwa.beginCallTrans();
          call != nwa.endCallTrans(); ++call)
      {
        sites.insert(call->third);
      }

      return sites.size();
    }

    size_t numExitSites(Nwa const & nwa)
    {
      std::set<Key> sites;

      for(Nwa::ReturnIterator return_ = nwa.beginReturnTrans();
          return_ != nwa.endReturnTrans(); ++return_)
      {
        sites.insert(return_->first);
      }

      return sites.size();
    }
    
    size_t numReturnSites(Nwa const & nwa)
    {
      std::set<Key> sites;

      for(Nwa::ReturnIterator return_ = nwa.beginReturnTrans();
          return_ != nwa.endReturnTrans(); ++return_)
      {
        sites.insert(return_->fourth);
      }

      return sites.size();
    }

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


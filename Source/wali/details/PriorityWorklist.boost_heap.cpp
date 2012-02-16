/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"

namespace wali
{
  namespace details
  {
    
#ifdef _MSC_VER
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
#endif
    

    template<typename Compare>
    BoostHeapPriorityWorklist<Compare>::BoostHeapPriorityWorklist()
      : Worklist<wfa::ITrans>()
      , workset(NotCompare(Compare()))
    {}

    template<typename Compare>
    BoostHeapPriorityWorklist<Compare>::~BoostHeapPriorityWorklist()
    {
      clear();
    }

    template<typename Compare>
    bool BoostHeapPriorityWorklist<Compare>::put( wfa::ITrans *t )
    {
      if( !t->marked() ) {
        t->mark();
        workset.push(t);
        return true;
      }
      else {
        return false;
      }
    }

    template<typename Compare>
    wfa::ITrans * BoostHeapPriorityWorklist<Compare>::get() 
    {
      wfa::ITrans* t = workset.top();
      workset.pop();
      return t;
    }

    template<typename Compare>
    bool BoostHeapPriorityWorklist<Compare>::empty() const
    {
      return workset.empty();
    }

    template<typename Compare>
    void BoostHeapPriorityWorklist<Compare>::clear()
    {
      typename pwl_t::iterator i = workset.begin();
      typename pwl_t::iterator iEND = workset.end();
      // unmark everything
      for( ; i != iEND ; i++ ) {
        (*i)->unmark();
      }
      workset.clear();
    }

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

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
        assert(t->worklistData() == NULL);
        t->mark();
        typename pwl_t::handle_type handle = workset.push(t);
        t->setWorklistData(new HeapHandleData(handle));
        //std::cout << "Adding trans @ " << t << std::endl;
        return true;
      }
      else {
        //std::cout << "Resifting trans @ " << t << std::endl;
        HeapHandleData* d = dynamic_cast<HeapHandleData*>(t->worklistData().get_ptr());
        assert(d != NULL);
        // Boost heaps are max-heaps, so we call increase-key instead of decrease-key.
        // FIXME: but if I call .increase(), it crashes. Huh?
        workset.update(d->handle);
        return false;
      }
    }

    template<typename Compare>
    wfa::ITrans * BoostHeapPriorityWorklist<Compare>::get() 
    {
      wfa::ITrans* t = workset.top();
      //std::cout << "Removing trans @ " << t << std::endl;
      t->setWorklistData(NULL);
      t->unmark();
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

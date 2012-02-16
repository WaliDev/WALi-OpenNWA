/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
#ifdef _MSC_VER
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
#endif

  template<typename Compare>
  PriorityWorklist<Compare>::PriorityWorklist()
    : Worklist<wfa::ITrans>()
    , workset(NotCompare(Compare()))
  {}

  template<typename Compare>
  PriorityWorklist<Compare>::~PriorityWorklist()
  {
    clear();
  }

  template<typename Compare>
  bool PriorityWorklist<Compare>::put( wfa::ITrans *t )
  {
    //std::cout << "\"Adding\" " << t->toString() << " to worklist...";
    if( !t->marked() ) {
      //std::cout << " (was not there before)\n";
      t->mark();
      workset.insert(t);
      return true;
    }
    else {
      //std::cout << "CALLING FIND (removing and re-adding)\n";
      // pwl_t::iterator loc = std::find(workset.begin(), workset.end(), t);
      // if (loc != workset.end()) {
      //   workset.erase(loc);
      //   workset.insert(t);
      // }
      // else {
      //   std::cout << "t marked but not in WL\n";
      //   assert(false);
      // }
      return false;
    }
  }

  template<typename Compare>
  wfa::ITrans * PriorityWorklist<Compare>::get() 
  {
    // static int invocation_count = 0;
    // if (invocation_count++ % 100000 == 0) {
    //   std::cout << invocation_count << " " << "worklist size: " << workset.size() << "\n";
    // }
    typename pwl_t::iterator i = workset.begin();
    wfa::ITrans* t = (*i);
    workset.erase(i);
    t->unmark();
    //std::cout << "Removing " << t->toString() << " from the worklist\n";
    return t;
  }

  template<typename Compare>
  bool PriorityWorklist<Compare>::empty() const
  {
    return workset.empty();
  }

  template<typename Compare>
  void PriorityWorklist<Compare>::clear()
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


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

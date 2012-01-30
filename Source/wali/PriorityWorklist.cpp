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
  PriorityWorklist::PriorityWorklist()
    : Worklist<wfa::ITrans>()
    , workset(LessThan(*this))
  {}

  PriorityWorklist::~PriorityWorklist()
  {
    clear();
  }

  bool PriorityWorklist::put( wfa::ITrans *t )
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

  wfa::ITrans * PriorityWorklist::get() 
  {
    // static int invocation_count = 0;
    // if (invocation_count++ % 100000 == 0) {
    //   std::cout << invocation_count << " " << "worklist size: " << workset.size() << "\n";
    // }
    pwl_t::iterator i = workset.begin();
    wfa::ITrans* t = (*i);
    workset.erase(i);
    t->unmark();
    //std::cout << "Removing " << t->toString() << " from the worklist\n";
    return t;
  }

  bool PriorityWorklist::empty() const
  {
    return workset.empty();
  }

  void PriorityWorklist::clear()
  {
    pwl_t::iterator i = workset.begin();
    pwl_t::iterator iEND = workset.end();
    // unmark everything
    for( ; i != iEND ; i++ ) {
      (*i)->unmark();
    }
    workset.clear();
  }

  int PriorityWorklist::compareTo( const wfa::ITrans* a , const wfa::ITrans* b) const
  {
    if( a->stack() == b->stack() ) {
      return 0;
    }
    else if( a->stack() < b->stack() ) {
      return -1;
    }
    else {
      return 1;
    }
    /*
       if( (a->stack() != WALI_EPSILON) && (b->stack() != WALI_EPSILON)) {
       return (a->stack() < b->stack()) ? -1 : 1;
       }
       else {
       return a->stack() && (b->stack() == WALI_EPSILON);
       }
       */
  }

}


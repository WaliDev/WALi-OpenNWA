/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
  PriorityWorklist::PriorityWorklist() : Worklist<wfa::ITrans>(), workset(LessThan(*this)) {}

  PriorityWorklist::~PriorityWorklist()
  {
    clear();
  }

  bool PriorityWorklist::put( wfa::ITrans *t )
  {
    if( !t->marked() ) {
      t->mark();
      workset.insert(t);
      return true;
    }
    else
      return false;
  }

  wfa::ITrans * PriorityWorklist::get() 
  {
    pwl_t::iterator i = workset.begin();
    wfa::ITrans* t = (*i);
    workset.erase(i);
    t->unmark();
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


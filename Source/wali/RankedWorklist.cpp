/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/RankedWorklist.hpp"
#include "wali/witness/Witness.hpp"

namespace wali
{
  RankedWorklist::RankedWorklist()
    : Worklist<wfa::ITrans>()
  {
  }

  RankedWorklist::~RankedWorklist()
  {
    clear();
  }

  bool RankedWorklist::put( wfa::ITrans *t )
  {
    if( !t->marked() ) {
      t->mark();
      int rank = rankOf(t);
      workset.insert(std::make_pair(rank, t));
      return true;
    }
    else
      return false;
  }

  wfa::ITrans * RankedWorklist::get() 
  {
    pwl_t::iterator i = workset.begin();
    assert(i != workset.end());
    wfa::ITrans* t = i->second;
    workset.erase(i);
    t->unmark();
    return t;
  }

  bool RankedWorklist::empty() const
  {
    return workset.empty();
  }

  void RankedWorklist::clear()
  {
    pwl_t::iterator i = workset.begin();
    pwl_t::iterator iEND = workset.end();
    // unmark everything
    for( ; i != iEND ; i++ ) {
      i->second->unmark();
    }
    workset.clear();
  }

  int RankedWorklist::rankOf(const wfa::ITrans* a ) const
  {
    witness::Witness * p = dynamic_cast<witness::Witness*>(a->weight().get_ptr());
    if (p != NULL) {
      return doRankOf(p->weight());
    }
    else {
      return doRankOf(a->weight());
    }
  }
}


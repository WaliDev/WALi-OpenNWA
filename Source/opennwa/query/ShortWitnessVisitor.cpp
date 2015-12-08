
#include "opennwa/query/ShortWitnessVisitor.hpp"

#include <iostream>
using std::cerr;

namespace opennwa {

  namespace query {

    using namespace wali;
    using namespace wali::witness;

    bool
    ShortWitnessVisitor::visit( Witness * w )
    {
      abort();
      fast_assert(false);
      m_answer = w;
      return false;
    }

    bool
    ShortWitnessVisitor::visitExtend( WitnessExtend * w )
    {
      fast_assert(w->hasLeft());
      fast_assert(w->hasRight());

      m_answer = NULL;
      w->left()->accept(*this);
      witness_t left = m_answer;

      m_answer = NULL;
      w->right()->accept(*this);
      witness_t right = m_answer;

      m_answer = left->extend(right.get_ptr());

      return false;
    }

    bool
    ShortWitnessVisitor::visitCombine( WitnessCombine * w )
    {
      // Track the minimum length of a child.
      unsigned long min_length = ULONG_MAX;
      unsigned long min_rank = ULONG_MAX;
      witness_t ret;

      for (std::list<witness_t>::const_iterator wit=w->children().begin();
           wit!=w->children().end();
           wit++)
      {
        assert ((*wit).is_valid() && "Invalid combine child pointer");

        m_answer = NULL;
        (*wit)->accept(*this);
        witness_t ptr = m_answer;

        unsigned long len = ptr->getMinimumLength();
        unsigned long rank = get_rank(ptr.get_ptr());

        if (rank < min_rank
            ||
            (rank == min_rank && len < min_length))
        {
          min_length = len;
          min_rank = rank;
          ret = ptr;
        }
      }

      assert (ret.is_valid() && "Invalid result of combine");

      m_answer = ret;

      return false;
    }

    bool
    ShortWitnessVisitor::visitRule( WitnessRule * w )
    {
      m_answer = w;
      return false;
    }

    bool
    ShortWitnessVisitor::visitTrans( WitnessTrans * w )
    {
      m_answer = w;
      return false;
    }

    bool
    ShortWitnessVisitor::visitMerge( WitnessMerge * w )
    {
      abort();
      fast_assert(false);
      m_answer = w;
      return false;
    }

  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


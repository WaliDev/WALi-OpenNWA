/* GT_EXTERNAL_LEGEND(2010) */

#ifndef WALI_DOMAINS_GENKILL_GEN_KILL_WEIGHT_NO_BOTTOM_INCLUDED
#define WALI_DOMAINS_GENKILL_GEN_KILL_WEIGHT_NO_BOTTOM_INCLUDED

#include <iostream>
#include <climits>
#include <cassert>

#include "wali/domains/genkill/GenKillBase.hpp"
#include "wali/SemElem.hpp"

namespace wali {
  namespace domains {
    namespace genkill {

      /*!
       * @class GenKillWeightNoBottom
       *
       * This is a templated class that implements the semiring needed for a
       * gen-kill dataflow-analysis problem. The template parameter must
       * implement the "Set" interface defined below
       *

       class Set {
         public:
           // Only Set constructor GenKillWeightNoBottom invokes
           Set(Set const &);

           static bool Eq(Set const & x, Set const & y);

           static Set Diff(Set const & x,
                           Set const & y,
                           bool normalizing = false);

           static Set Union(Set const & x, Set const & y);

           static Set Intersect(Set const & x, Set const & y);

           static Set const & EmptySet();

           std::ostream & print(std::ostream & o);
       };

       The above exact signatures are only recommended; for instance, the
       functions could also take non-const references (but why would you do
       that to yourself?) or by value; or they could take other optional
       arguments; Diff could be overloaded instead of using a default
       parameter, etc.

       For normal elements, a semiring element is represented by a pair of
       sets (k,g), which have the meaning \x.(x - k) union g.

       Note that if k and g were allowed to be arbitrary sets, it would
       introduce redundant elements into the domain: e.g., ({a,b}, {c,d})
       would have the same meaning as ({a,b,c,d}, {c,d}).  Therefore,
       GenKillWeightNoBottom maintains an invarient that (k intersect g = empty),
       and the operation that builds a semiring element performs the
       normalization (k, g) |-> (k-g, g). When computing (k-g) for purposes
       of this normalization, the Set::Diff function is called with
       'normalizing' equal to 'true.' (At other times, e.g. in 'extend', this
       parameter is left out.) See GenKillWeight for commentary about why
       this might be useful.

       The special elements are
           1. zero
           2. one = (emptyset, emptyset) = \x.x
       Note that zero is a different element from the element (Univ,
       emptyset); it is not just a transformer with specific gen & kill sets,
       it is a completely special value.

       The implementation maintains unique representations for zero and one.
      */
      template< typename Set >
      class GenKillWeightNoBottom
        : public GenKillBase<GenKillWeightNoBottom<Set>, Set>
      {
      private: // methods -----------------------------------------------------------

        typedef GenKillBase<GenKillWeightNoBottom<Set>, Set> BaseClass;
        friend class GenKillBase<GenKillWeightNoBottom<Set>, Set>; // god c++ is stupid sometimes

        // Constructors
        // The constructors are private to ensure uniqueness of one, zero, and bottom

        // Constructor for legitimate values
        GenKillWeightNoBottom(const Set& k, const Set& g, unsigned int c=0)
          : BaseClass(k, g, c)
        {}

        // Constructor for zero
        GenKillWeightNoBottom(unsigned int c=0)
          : BaseClass(c)
        {}

      };



    } // namespace genkill
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

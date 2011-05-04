#ifndef wali_nwa_query_details_FILTER_HPP
#define wali_nwa_query_details_FILTER_HPP

#include "wali/nwa/NWA.hpp"

#include <utility>

namespace wali
{
  namespace nwa
  {
    namespace query
    {
      namespace details
      {
        /// Defines a generic predicate.
        ///
        /// Predicates take a transition and return true or false depending
        /// on whether the predicate holds for that transition or not.
        namespace predicates
        {

          /// This class takes a selector and an object. It returns true for
          /// a transition if the selector returns that object.
          template<typename Selector>
          class SelectorEqualityPredicate
          {
          private:
            typedef typename Selector::type EqualityType;

            Selector selector;
            EqualityType mark;

          public:
            SelectorEqualityPredicate(Selector sel, EqualityType const & goal)
              : selector(sel)
              , mark(goal)
            {}
            
            bool operator() (NWA::Call const & c) {
              return mark == selector(c);
            }

            bool operator() (NWA::Return const & r) {
              return mark == selector(r);
            }

          };


          /// Like std::make_pair. Returns a SelectorEqualityPredicate of the
          /// appropriate type.
          template<typename Selector, typename EqualityType>
          SelectorEqualityPredicate<Selector>
          makeSelectorEqualityPredicate(Selector selector, EqualityType const & goal)
          {
            return SelectorEqualityPredicate<Selector>(selector, goal);
          }
        }

        
        /// This namespace defines filtering iterators.
        ///
        namespace filter
        {

          
          
        }
      }
    }
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif

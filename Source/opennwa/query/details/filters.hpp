#ifndef wali_nwa_query_details_FILTER_HPP
#define wali_nwa_query_details_FILTER_HPP

#include "opennwa/Nwa.hpp"

#include <iterator>
#include <utility>

namespace opennwa
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

#if 0
          template<typename Selector>
          struct SelectorTraits
          {
            typedef typename Selector::type type;
          };

          template<typename Object>
          struct SelectorTraits<Object*>
          {
            typedef
#endif

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

            template<typename T>
            bool operator() (T const & c) {
              return mark == selector(c);
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

          template<typename CPlusPlusIterator>
          class IteratorPairIterator
          {
          public:
            typedef std::forward_iterator_tag iterator_category;
            typedef typename std::iterator_traits<CPlusPlusIterator>::value_type value_type;
            typedef typename std::iterator_traits<CPlusPlusIterator>::pointer pointer;
            typedef typename std::iterator_traits<CPlusPlusIterator>::reference reference;
            typedef typename std::iterator_traits<CPlusPlusIterator>::difference_type difference_type;

            
          private:
            CPlusPlusIterator current;
            CPlusPlusIterator end;

          public:
            IteratorPairIterator(CPlusPlusIterator left, CPlusPlusIterator right)
              : current(left)
              , end(right)
            {}

            bool atEnd() {
              return current == end;
            }

            void operator++() {
              ++current;
            }

            bool operator==(IteratorPairIterator i) {
              return this->current == i->current;
            }

            value_type operator*() {
              return *current;
            }
          };
          

          template<typename BackingIterator, typename Predicate>
          class FilteringIterator
          {
          public:
            typedef std::forward_iterator_tag iterator_category;
            typedef typename std::iterator_traits<BackingIterator>::value_type value_type;
            typedef typename std::iterator_traits<BackingIterator>::pointer pointer;
            typedef typename std::iterator_traits<BackingIterator>::reference reference;
            typedef typename std::iterator_traits<BackingIterator>::difference_type difference_type;
            
            
          private:
            BackingIterator backing;
            Predicate predicate;

          public:
            FilteringIterator(BackingIterator start, Predicate pred)
              : backing(start)
              , predicate(pred)
            {
              if (!atEnd() && !predicate(*backing)) {
                advance();
              }
            }

            void advance() {
              // Advance the backing iterator until either we reach the end
              // of that sequence, or the predicate no longer holds.
              do {
                ++backing;
              } while (!backing.atEnd() && !predicate(*backing));
            }

            bool atEnd() {
              // We're at the end IFF our backing iterator is at the end
              return backing.atEnd();
            }

            void operator++() {
              advance();
            }

            bool operator==(FilteringIterator const & other) {
              return this->backing == other.backing;
            }

            value_type operator*() {
              return *backing;
            }
          };
          

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

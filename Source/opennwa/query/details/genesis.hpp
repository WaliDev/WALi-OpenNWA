#define wali_nwa_query_details_GENESIS_HPP

#ifndef wali_nwa_query_details_GENESIS_HPP
#define wali_nwa_query_details_GENESIS_HPP

#include "opennwa/Nwa.hpp"
#include "opennwa/query/details/iterators.hpp"

#include <utility>

namespace opennwa
{
    namespace query
    {
      namespace details
      {
        /// This namespace defines 
        namespace genesis
        {

          /// This turns return transitions into call transitions, mapping
          /// the symbols that correspond and dropping the call predecessor.
          struct ReturnTransitionToFakeCallTransformer
          {
            NWA::Call operator()(NWA::Return const & r) const
            {
              return NWA::Call(r.first, r.third, r.fourth);
            }
          };


          struct FromCallTransitions
          {
            typedef NWA::CallIterator Iterator;

            std::pair<Iterator, Iterator> getTransitions(NWA const & nwa) const
            {
              return std::make_pair(nwa.beginCallTrans(), nwa.endCallTrans());
            }
          };


          struct FromInternalTransitions
          {
            typedef NWA::InternalIterator Iterator;

            std::pair<Iterator, Iterator> getTransitions(NWA const & nwa) const
            {
              return std::make_pair(nwa.beginInternalTrans(), nwa.endInternalTrans());
            }
          };


          struct FromReturnTransitions
          {
            typedef NWA::ReturnIterator Iterator;

            std::pair<Iterator, Iterator> getTransitions(NWA const & nwa) const
            {
              return std::make_pair(nwa.beginReturnTrans(), nwa.endReturnTrans());
            }
          };


          struct FromAllTransitions
          {
            typedef iterators::iterator_base_wrapper<NWA::Call> _base_iterator;
            typedef iterators::iterator_sequence<_base_iterator> Iterator;

            std::pair<Iterator, Iterator> getTransitions(NWA const & nwa) const
            {
              Iterator sequence;

              // Calls
              iterators::cplusplus_iterator_adapter<NWA::CallIterator>
                callBegin(nwa.beginCallTrans()),
                callEnd(nwa.endCallTrans());
                
              sequence.append_range(_base_iterator(callBegin),
                                    _base_iterator(callEnd));

              // Internals
              iterators::cplusplus_iterator_adapter<NWA::InternalIterator>
                internalBegin(nwa.beginInternalTrans()),
                internalEnd(nwa.endInternalTrans());
                
              sequence.append_range(_base_iterator(internalBegin),
                                    _base_iterator(internalEnd));

              // Returns

              return std::make_pair(sequence, Iterator());
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

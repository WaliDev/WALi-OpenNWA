#ifndef wali_nwa_query_details_SELECTORS_HPP
#define wali_nwa_query_details_SELECTORS_HPP

#include "opennwa/Nwa.hpp"

#include <utility>

namespace opennwa
{
    namespace query
    {
      namespace details
      {
        /// This namepsace defines a library of selector classes.
        ///
        /// A selector class's purpose is to transform an NWA transition into
        /// whatever the user wants. For instance, if the user writes
        ///    query().select_symbol(). ...
        /// then the 'selector' is what's responsible for pulling out the
        /// symbol field of each transition in order to fulfill that query.
        ///
        /// A selector is a functor that is callable on one or more
        /// transition types -- calls, internals, and returns -- and returns
        /// pretty much anything you want. For most things, you'll want it to
        /// be callable on all three types, which means you'll have to
        /// overload operator(). (This is why most of these must be classes
        /// instead of using function pointers.) Note however that because
        /// C/C++ doesn't have strong typedefs, Call and Internal transitions
        /// are the same type! This means that you must do the same thing in
        /// both cases.
        ///
        /// Right now there is also a 'type' field in each class that holds
        /// the type of whatever the user wants out, but really this should
        /// be reworked slightly to use traits. (That way the user can pass a
        /// function if they really want, with the caveat that you can't do
        /// the overload thing.)
        namespace selectors
        {
          /// This class selects the whole transition
          struct CallTransitionSelector
          {
            typedef Nwa::Call type;

            Nwa::Call operator()(Nwa::Call const & c) const {
              return c;
            }
          };

          /// This class selects the whole transition
          struct InternalTransitionSelector
          {
            typedef Nwa::Internal type;

            Nwa::Internal operator()(Nwa::Internal const & c) const {
              return c;
            }
          };


          /// This class selects the whole transition
          struct ReturnTransitionSelector
          {
            typedef Nwa::Return type;

            Nwa::Return operator()(Nwa::Return const & c) const {
              return c;
            }
          };


          /// This class selects the source from transitions
          struct SourceSelector
          {
            /// The type of what it returns -- SourceSelectors select the
            /// source, so that's a state.
            typedef State type;

            State operator()(Nwa::Call const & c) const {
              return c.first;
            }

            //State operator()(Nwa::Internal const & i) const {
            //  return i.first;
            //}

            State operator()(Nwa::Return const & r) const {
              return r.first;
            }
          };


          /// This class selects the symobl from transitions
          struct SymbolSelector
          {
            /// SymbolSelectors select the symbol, which is a Symbol
            typedef Symbol type;

            State operator()(Nwa::Call const & c) const {
              return c.second;
            }

            //State operator()(Nwa::Internal const & i) const {
            //  return i.second;
            //}

            State operator()(Nwa::Return const & r) const {
              return r.third;
            }
          };
        
        
          /// This class selects the target from transitions
          struct TargetSelector
          {
            typedef State type;

            State operator()(Nwa::Call const & c) const {
              return c.third;
            }

            //State operator()(Nwa::Internal const & i) const {
            //  return i.third;
            //}

            State operator()(Nwa::Return const & r) const {
              return r.fourth;
            }
          };


          /// This class selects the call predecessor from return transitions
          struct CallPredecessorSelector
          {
            typedef State type;

            State operator()(Nwa::Return const & r) const {
              return r.second;
            }
          };


          /// This selects a pair of things
          template<typename Selector1, typename Selector2>
          struct PairSelector
          {
            // The types that the two selectors select
            typedef typename Selector1::type T1;
            typedef typename Selector2::type T2;

            // The type *this* selector selects
            typedef std::pair<T1, T2> type;

            type operator()(Nwa::Call const & c) const {
              return type(Selector1()(c),
                          Selector2()(c));
            }

            //type operator()(Nwa::Internal const & c) const {
            //  return type(Selector1()(c),
            //              Selector2()(c));
            //}

            type operator()(Nwa::Return const & c) const {
              return type(Selector1()(c),
                          Selector2()(c));
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

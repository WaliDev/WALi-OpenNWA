#include "opennwa/traverse/depth.hpp"
#include "opennwa/NWA.hpp"

namespace wali {
  namespace nwa {
    namespace experimental {


      class TransCopier : public traverse::ConstCallStringTransitionFunctor
      {
        NWA & dest_nwa;

        State getPrefixedName(CallString const & cs, State state) {
          State result = cs[0];

          for (unsigned i=1; i<cs.size(); ++i) {
            result = getKey(result, cs[i]);
          }

          return getKey(result, state);
        }

        State getPrefixedName(CallString const & cs, State call, State state) {
          State result = cs[0];

          for (unsigned i=1; i<cs.size(); ++i) {
            result = getKey(result, cs[i]);
          }

          result = getKey(result, call);
          result = getKey(result, state);

          return result;
        }

        
      public:
        TransCopier(NWA & destination_nwa)
          : dest_nwa(destination_nwa)
        {}

        
        virtual void doInternal(NWA const & nwa, CallString const & cs,
                                State source, Symbol symbol, State target)
        {
          (void) nwa;
          dest_nwa.addInternalTrans(getPrefixedName(cs, source),
                                    symbol,
                                    getPrefixedName(cs, target));
        }

        
        virtual void doCall(NWA const & nwa, CallString const & cs,
                            State source, Symbol symbol, State target)
        {
          (void) nwa;
          dest_nwa.addCallTrans(getPrefixedName(cs, source),
                                symbol,
                                getPrefixedName(cs, source, target));
        }

        
        virtual void doReturn(NWA const & nwa, CallString const & cs,
                              State source, State pred, Symbol symbol, State target)
        {
          (void) nwa;
          dest_nwa.addReturnTrans(getPrefixedName(cs, pred, source),
                                  getPrefixedName(cs, pred),
                                  symbol,
                                  getPrefixedName(cs, target));
        }
      };
      
      
      NWARefPtr expandNwa(NWA const & nwa)
      {
        NWARefPtr out = new NWA();
        TransCopier copier(*out);

        dfsCallStringTraversal(nwa, NULL, &copier);

        return out;
      }
      
    }
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

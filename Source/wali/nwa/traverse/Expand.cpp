#include "wali/nwa/traverse/depth.hpp"

#include "wali/nwa/NWA.hpp"

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
        
      public:
        TransCopier(NWA & destination_nwa)
          : dest_nwa(destination_nwa)
        {}

        
        virtual void doInternal(NWA const & nwa, CallString const & cs,
                                State source, Symbol symbol, State target)
        {
          (void) nwa;
          source = getPrefixedName(cs, source);
          target = getPrefixedName(cs, target);
          
          dest_nwa.addInternalTrans(source, symbol, target);
        }

        
        virtual void doCall(NWA const & nwa, CallString const & cs,
                            State source, Symbol symbol, State target)
        {
          (void) nwa;
          source = getPrefixedName(cs, source);
          target = getPrefixedName(cs, target);
          
          dest_nwa.addCallTrans(source, symbol, target);
        }

        
        virtual void doReturn(NWA const & nwa, CallString const & cs,
                              State source, State pred, Symbol symbol, State target)
        {
          (void) nwa;
          source = getPrefixedName(cs, source);
          pred = getPrefixedName(cs, pred);
          target = getPrefixedName(cs, target);
          
          dest_nwa.addReturnTrans(source, pred, symbol, target);
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

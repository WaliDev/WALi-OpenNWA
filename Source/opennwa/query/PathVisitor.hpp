
#include <map>
#include <list>

#include "opennwa/NwaFwd.hpp"
#include "opennwa/NestedWord.hpp"
#include "wali/witness/Visitor.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessTrans.hpp"

namespace opennwa {

  namespace query {

    class PathVisitor : public wali::witness::Visitor {
        
    private:
      /// Holds the NWA associated with the given query. We need this so
      /// that when we know that the trace went from state p to q, we can
      /// look up a symbol on that edge.
      Nwa const & nwa;

      /// This is the word that corresponds to the trace; built up as
      /// visitRule() is called.
      NestedWord word;

      /// Holds the *PDS* state and stack symbol corresponding to the
      /// "first half" of a NWA return transition. See visitRule() for
      /// more details.
      // %%% Fix comment, possibly make this more efficient.
      std::map<wali::Key,wali::Key> states;
        
    
    public:
      
      PathVisitor(Nwa const & orig)
        : nwa(orig)
      {}
      
      ~PathVisitor() {}

      virtual bool visit( wali::witness::Witness * w ) {
        (void) w;
        return true;
      }
  
      virtual bool visitExtend( wali::witness::WitnessExtend * w ) {
        (void) w;
        return true;
      }
  
      virtual bool visitCombine( wali::witness::WitnessCombine * w ) {
        (void) w;
        return true;
      }

      /// This is basically called once for each WPDS rule taken on the
      /// PDS's "accepting" path. Builds up 'word' so it contains the
      /// actual NWA word.
      virtual bool visitRule( wali::witness::WitnessRule * w );

      NestedWord getPath() {return word;}

      virtual bool visitTrans( wali::witness::WitnessTrans * w ) {
        (void) w;
        return true;
      }
  
      virtual bool visitMerge( wali::witness::WitnessMerge * w ) {
        (void) w;
        return true;
      }
  
    };
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


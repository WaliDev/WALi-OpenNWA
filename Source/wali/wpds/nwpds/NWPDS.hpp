#ifndef wali_wpds_nwpds_NWPDS_GUARD
#define wali_wpds_nwpds_NWPDS_GUARD 1

/**
 * @author Prathmesh Prabhu
 */

// ::wali
#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
// ::wali::wpds
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wpds/RuleFunctor.hpp"
// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/EWPDS.hpp"
// ::wali::wfa
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/ITrans.hpp"
// ::std
#include <vector>
// ::std::tr1
#include <tr1/unordered_map>
// ::std (for pair)
#include <utility>

namespace wali
{
  namespace wfa
  {
    class WFA;
  }

  namespace wpds 
  {
    class Wrapper;

    namespace nwpds 
    {

      class NWPDS : public wpds::ewpds::EWPDS
      {
        public:
          /**
           * Newton Solver creates copies of some stack symbols in order to store values
           * computed in the last Newton iteration.
           * This keeps a map from the original Key to the Key_NEWTON_OLDVAL, generated within the
           * solver (call to poststar/prestar).
           **/
          typedef std::tr1::unordered_map< wali::Key,wali::Key > Key2KeyMap;
          /**
           * Newton Solver creates copies of some generated states  in the WFA,
           * in order to store function summaries from the last Newton
           * iteration.
           * Ideally, we would like to store a map from generated states to summary generated states, but
           * we can't create these states early enough.
           **/
          typedef std::pair< wali::Key, wali::Key > IntState;
          struct IntStateHash{
            std::size_t operator () (IntState const& s) const {
              return (s.first << (sizeof(std::size_t)/2)) & s.second;
            }
          };
          typedef std::tr1::unordered_map< IntState, IntState, IntStateHash> State2StateMap;
          /**
            Short name:
           **/
          typedef std::vector < wali::wfa::ITrans* > worklist_t;

        public:
          /**
           * For parsing XML NWPDSs.
           * @shadow wali::wpds::nwpds::FWPDS::XMLTag
           */
          static const std::string XMLTag;

        public:
          NWPDS(bool dbg=false);
          NWPDS(ref_ptr<Wrapper> wrapper, bool dbg=false);
          NWPDS( const NWPDS& f );
          ~NWPDS();

          ///////////
          // pre*
          ///////////
          virtual void prestar( wfa::WFA const & input, wfa::WFA & output );

          virtual wfa::WFA prestar( wfa::WFA const & input) {
            wfa::WFA output;
            prestar(input,output);
            return output;
          }

          virtual void poststar( wfa::WFA const & input, wfa::WFA & output );

          virtual wfa::WFA poststar( wfa::WFA const & input) {
            wfa::WFA output;
            poststar(input,output);
            return output;
          }

        protected:
          virtual void update(
              Key from,
              Key stack,
              Key to,
              sem_elem_t se,
              Config * cfg);
          virtual wfa::ITrans* update_prime(
              Key from,
              wfa::ITrans* call,
              rule_t r, 
              sem_elem_t delta, 
              sem_elem_t wWithRule );

#if 0
          /**
           * Changes some rules of this PDS to actually solve the linearized prestar problem. 
           **/
          void prestarSetupPds();
#endif
          /**
           * Changes some rules of this PDS to actually solve the linearized poststar problem. 
           **/
          void poststarSetupPds();
          /**
           * After each linearized prestar/poststar, checks if the fa has
           * changed, if yes, it updates the stored const values on the FA.
           **/
          void poststarUpdateFa(wali::wfa::WFA& f);
          /**
           * Restores PDS to its former ruleset after prestar/poststar
           **/
          void restorePds();
          /**
           * Sets the weights on transitions in outfa that correspond to constants in the fix point calculation
           * to zero
           **/
          void cleanUpFa(wali::wfa::WFA& fa);

          /**
           * Creates a map from new to old key if needed, and returns the old key for the new key.
           **/
          wali::Key getOldKey(wali::Key newKey);

        protected:

          /**
           * @briend : calls EWPDS::poststarSetupFixpoint, and a little book-keeping for NWPDS.
           **/
          void poststarSetupFixpoint(wali::wfa::WFA const & input, wali::wfa::WFA& fa);

        private:

          /**
           * This map is used to make create unique stack symbols for newton
           * constants for stack symbols at calls. 
           **/
          Key2KeyMap stack2ConstMap;

          /**
           * Used by poststar to map stack symbols for call sites to the constant symbol for fake call.
           * If there is a rule: <p,y> --> <p',y'y''>, replace by <p,t> --> <p',y'y''> 
           * and oldCallSite[y] = t;
           **/
          Key2KeyMap oldCallSite;

          /**
           * Used by poststar to map generated states for function entries to the constant symbol for summary calls
           * If there is a rule: <p,t> --> <p',y'y''>, replace by <p,y> --> <p',t'y''> 
           * and oldFuncEntry[(p',y')] = (p',t')
           * We can't generate these wali::Key's early enough, so we temporarily store pairs.
           **/
          State2StateMap oldFuncEntryPair;
          Key2KeyMap oldFuncEntry;

          /**
           * This worklist is used to remember what transitions were modified in one
           * Newton Step. Only those are checked for changed values to be copied over
           * to the Newton constant transitions.
           **/
          worklist_t newtonWl;

          /**
           * We modify the pds to linearize the equations to be solved.
           * This vector stores the rules that must be restored after we're done.
           **/
          std::vector<rule_t> savedRules;

          /**
           * The set of rules added by NWPDS.
           * These must be reduced after the query to leave the PDS unchanged
           **/
          std::vector<rule_t> addedRules;

          /**
           * Facilitate printing by creating a NWPDS with dbg set to true.
           **/
          bool dbg;
      };

      /**
       * 
       * @class Delta2Rules
       *
       * Gets all delta_2 rules from the wpds. Holds on to const rule_t
       * so that it can be cast down to the correct ERule/Rule type.
       */
      class Delta2Rules: public RuleFunctor
      {
        public:
          std::vector< rule_t > rules;
          Delta2Rules();
          virtual ~Delta2Rules();
          virtual void operator() (rule_t & r);
      };

      class RemoveOldTrans : public wali::wfa::TransFunctor
      {
        public:
          RemoveOldTrans(const NWPDS::Key2KeyMap& oldStackMap, const NWPDS::Key2KeyMap& oldStateMap);
          virtual void operator() (wali::wfa::ITrans * it);
        private:
          const NWPDS::Key2KeyMap& oldStackMap;
          const NWPDS::Key2KeyMap& oldStateMap;
      };

      class CreateInitialNewtonWl : public wali::wfa::TransFunctor
      {
        public:
          CreateInitialNewtonWl(NWPDS::worklist_t& wl);
          virtual void operator () (wali::wfa::ITrans * it);
        private:
          NWPDS::worklist_t& wl;
      };
    } //namespace nwpds
  } //namespace wpds
} //namespae wali
#endif //wali_wpds_newpds_NWPDS_GUARD

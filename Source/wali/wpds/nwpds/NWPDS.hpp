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
          typedef std::map< wali::Key,wali::Key > Key2KeyMap;
          /**
            Short name:
          **/
          //typedef wali::ref_ptr< wali::Worklist< wali::wfa::ITrans > > worklist_t;
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
          virtual void poststarSetupFixpoint(wfa::WFA const & input, wfa::WFA& fa);
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

            /**
             * Changes some rules of this PDS to actually solve the linearized prestar problem. 
             **/
            void prestarSetupPds();
            /**
             * Changes some rules of this PDS to actually solve the linearized poststar problem. 
             **/
            void poststarSetupPds();
            /**
             * After each linearized prestar/poststar, checks if the fa has
             * changed, if yes, it updates the stored const values on the FA.
             **/
            void updateFa(wali::wfa::WFA& f);
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


        private:

          Key2KeyMap stack2ConstMap;
          Key2KeyMap state2ConstMap;
          Key2KeyMap state2StackMap;

          /**
           * Remember states generated for Newton Steps in the fa
           * We need to clean these up when returning the fa
           **/
          typedef std::vector< std::pair< wali::Key, std::pair<wali::Key, wali::Key> >  > GenStates;
          GenStates genStates;

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

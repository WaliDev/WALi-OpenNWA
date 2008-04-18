#ifndef wali_wpds_fwpds_SWPDS_GUARD
#define wali_wpds_fwpds_SWPDS_GUARD 1

#include <map>
#include "wali/Common.hpp"

#include "wali/wpds/RuleFunctor.hpp"

#include "wali/wpds/ewpds/EWPDS.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"

#include "wali/graph/GraphCommon.hpp"
#include "wali/graph/InterGraph.hpp"
#include "wali/graph/Graph.hpp"
#include "wali/graph/SummaryGraph.hpp"

namespace wali {

  namespace wfa {
    class WFA;
    class Trans;
  }

  namespace wpds {

    namespace ewpds {
      
      /*!
       * @class CopyCallRules
       * Inserts call rules into a EWPDS
       */
      class CopyCallRules : public RuleFunctor
      {
        EWPDS &pds;
        
      public:
        CopyCallRules( EWPDS &w );
        virtual ~CopyCallRules();
        
        virtual void operator()( rule_t & r );
      };
    } // namespace ewpds

    namespace fwpds {

      class SWPDS : public FWPDS {
        public:
          /*!
           * For parsing XML FWPDSs.
           * @shadow wali::wpds::ewpds::EPDS::XMLTag
           */
          static const std::string XMLTag;


      public:
        SWPDS();
        SWPDS(Wrapper *wrapper);
        virtual ~SWPDS();

        virtual void prestar( wfa::WFA &input, wfa::WFA &output);
        virtual void poststar( wfa::WFA &input, wfa::WFA &output);

        void nonSummaryPrestar( wfa::WFA &input, wfa::WFA &output);
        void nonSummaryPoststar( wfa::WFA &input, wfa::WFA &output);

        void addEntryPoint(Key e);
        void preprocess();
        bool reachable(Key k);
        bool multiple_proc(Key k);

      private:
        virtual bool make_rule(
            Config *f,
            Config *t,
            Key stk2,
            sem_elem_t se,
            rule_t& r );

      private:
        WpdsStackSymbols syms;
        bool preprocessed;
        EWPDS pre_pds;
        graph::SummaryGraph *sgr;
      }; // class SWPDS

    } // namespace fwpds

  } // namespace wpds

} // namespace wali

#endif // wali_wpds_fwpds_SWPDS_GUARD


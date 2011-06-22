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

namespace wali 
{

  namespace wfa 
  {
    class WFA;
    class Trans;
  }

  namespace wpds 
  {

    namespace ewpds 
    {
      
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

    namespace fwpds 
    {

      class SWPDS : public FWPDS {
        public:
          /*!
           * For parsing XML FWPDSs.
           * @shadow wali::wpds::ewpds::EPDS::XMLTag
           */
          static const std::string XMLTag;


      public:
        SWPDS();
        SWPDS(ref_ptr<Wrapper> wrapper);
        virtual ~SWPDS();

        virtual void prestar( wfa::WFA const & input, wfa::WFA &output);
        virtual wfa::WFA prestar( wfa::WFA const & input) {
          return this->EWPDS::prestar(input);
        }

        virtual void poststar( wfa::WFA const & input, wfa::WFA &output);
        virtual wfa::WFA poststar( wfa::WFA const & input) {
          return this->EWPDS::poststar(input);
        }

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
	    bool replace_weight,
            rule_t& r );

        ////////////////////////
        // Forwarding function to allow compilation with
        // -Woverloaded-virtual. See a couple comments in EWPDS.hpp for more.
        virtual bool make_rule(
            Config *f,
            Config *t,
            Key stk2,

            rule_t& r ) {
          return this->FWPDS::make_rule(f, t, stk2, r);
        }

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


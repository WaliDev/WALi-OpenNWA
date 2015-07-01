#ifndef wali_wpds_fwpds_FWPDS_GUARD
#define wali_wpds_fwpds_FWPDS_GUARD 1

/**
 * @author Nicholas Kidd
 * @author Akash Lal
 *
 */

#include "wali/Common.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Wrapper.hpp"

#include "wali/wpds/ewpds/EWPDS.hpp"

#include "wali/graph/GraphCommon.hpp"
#include "wali/graph/InterGraph.hpp"
#include "wali/graph/RegExp.hpp"
namespace wali {

  namespace wfa {
    class WFA;
    class Trans;
  }

  namespace graph {
    class InterGraph;
    class RegExp;
  }

  namespace wpds {

    class Wrapper;
    class rule_t;

    namespace fwpds 
    {

      class FWPDS : public ewpds::EWPDS 
      {
        typedef ref_ptr<graph::RegExp> reg_exp_t;

        public:
          /**
           * For parsing XML FWPDSs.
           * @shadow wali::wpds::ewpds::EPDS::XMLTag
           */
          static const std::string XMLTag;

        public:
          FWPDS();
          FWPDS(ref_ptr<Wrapper> wrapper);
          FWPDS( const FWPDS& f );
          ~FWPDS();


          ////////////
          // Constrol Solver behaviour
          ///////////
          ///Must be called before the graph based solvers are setup.
          ///So, call before calling poststar. I  don't think works with prestar.
          void useNewton(bool set);
          /// Must be called before the graph based solvers are setup.
          //So, call before calling poststar (does not work with prestar).
          void useNewtonNoTensor(bool set);
          
          // Newton can leave the output automaton with either tensored weights or not.
          bool isOutputTensored();
          ////////////
          // add rules
          ////////////

          // Inherited from EWPDS

          ///////////
          // pre*
          ///////////
          virtual void prestar( wfa::WFA const & input, wfa::WFA & output );

          virtual wfa::WFA prestar( wfa::WFA const & input) {
              return this->EWPDS::prestar(input);
          }


          ///////////
          // post*
          ///////////
          virtual void poststar( wfa::WFA const & input, wfa::WFA & output );

          virtual wfa::WFA poststar( wfa::WFA const & input) {
              return this->EWPDS::poststar(input);
          }

          double getOutRegExps( wfa::WFA const & input, wfa::WFA & output, map<int,reg_exp_t> & outNodeRegExps, map<int,int> & uMap, map<int,int> & oMap, map<int,std::pair< std::pair<int,int> ,int > > & mapBack, std::map<std::pair<std::pair<int,int>,int>,int> & transMap, vector<int> & eps, map<std::pair<int,int>,std::pair<int,int>> & mergeSrcMap);
		  double getOutRegExpsSimple(wfa::WFA const & input, wfa::WFA & output, map<int, reg_exp_t> & outNodeRegExps);
		  void poststarIGR( wfa::WFA const & input, wfa::WFA & output );

          ///////////////////////
          // FWPDS Settings
          //////////////////////

          /** @brief Sets evaluation strategy for RegExp after InterGraph
           * saturation is complete, i.e., during the time when
           * transition weights are calculated. It is true by default,
           * but setting it to false seems to be far more efficient
           * while using BDD-based weight domain (Moped)
           */
          void topDownEval(bool f);

        private:
          void prestar_handle_call(
              wfa::ITrans *t1,
              wfa::ITrans *t2,
              rule_t &r,
              sem_elem_t delta
              );

          void prestar_handle_trans(
              wfa::ITrans * t,
              wfa::WFA & ca  ,
              rule_t & r,
              sem_elem_t delta );

          void poststar_handle_eps_trans(
              wfa::ITrans *teps, 
              wfa::ITrans *tprime,
              sem_elem_t delta);

          void poststar_handle_trans(
              wfa::ITrans * t ,
              wfa::WFA & ca   ,
              rule_t & r,
              sem_elem_t delta
              );

          void update(
              wali::Key from
              , wali::Key stack
              , wali::Key to
              , sem_elem_t se
              , Config * cfg
              );

          wfa::ITrans * update_prime(
              Key from, //<! Guaranteed to be a generated state
              wfa::ITrans* call, //<! The call transition
              rule_t r, //<! The push rule
              sem_elem_t delta, //<! Delta change on the call transition
              sem_elem_t wWithRule //<! delta \extends r->weight()
              );

          void operator()( wfa::ITrans const * orig );

          ///////////
          // helpers
          ///////////
          bool checkResults( wfa::WFA const & input, bool poststar );


        protected:
          sem_elem_t wghtOne;
          std::vector< graph::InterGraphPtr > interGrs;
          graph::InterGraphPtr interGr;
          bool checkingPhase;
          // If set, use InterGraph::setupNewtonSolution for poststar
          bool newton;
          // If set, use InterGraph::setupNewtonNoTensorSolution for poststar
          bool newtonNoTensor;
          bool topDown;

      }; // class FWPDS

    } // namespace fwpds

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_fwpds_FWPDS_GUARD


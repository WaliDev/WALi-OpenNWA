/**
 * @author Nicholas Kidd
 * @author Akash Lal
 *
 * @version $Id$
 *
 */

#include "wali/Worklist.hpp"
#include "wali/Common.hpp"
#include "wali/HashMap.hpp"
#include "wali/SemElem.hpp"
#include "wali/KeyPairSource.hpp"

// ::wali::util
#include "wali/util/Timer.hpp"

// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/TransFunctor.hpp"

// ::wali::wpds
#include "wali/wpds/Config.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/wpds/Rule.hpp"

// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/ERule.hpp"
#include "wali/wpds/ewpds/ETrans.hpp"

// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/fwpds/LazyTrans.hpp"

// ::wali::graph
#include "wali/graph/RegExp.hpp"
#include "wali/graph/InterGraph.hpp"

using namespace wali;
using namespace wali::graph;
using namespace wali::wpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::ewpds;

#define FWPDS_DYN_CAST 0

const std::string FWPDS::XMLTag("FWPDS");

FWPDS::FWPDS() : EWPDS(), interGr(NULL), checkingPhase(false)
{
}

FWPDS::FWPDS(ref_ptr<wpds::Wrapper> wr) : EWPDS(wr) , interGr(NULL), checkingPhase(false)
{
}

FWPDS::FWPDS( const FWPDS& f ) : EWPDS(f),interGr(NULL),checkingPhase(false)
{
}

///////////////////////////////////////////////////////////////////
void FWPDS::topDownEval(bool f) {
  graph::RegExp::topDownEval(f);
}

struct FWPDSCopyBackFunctor : public wfa::TransFunctor
{
  graph::InterGraphPtr gr;
  FWPDSCopyBackFunctor(graph::InterGraphPtr _gr) : gr(_gr) {}
  virtual void operator()( wfa::ITrans* t ) {
    LazyTrans *lt = static_cast<LazyTrans *> (t);
    lt->setInterGraph(gr);
    
    if (!wali::is_lazy_fwpds()) {
      // Call to compute the weight
      lt->weight();
    }
    
  }
};

struct FWPDSSourceFunctor : public wfa::TransFunctor
{
  graph::InterGraph & gr;
  bool post;

  FWPDSSourceFunctor( graph::InterGraph & g, bool p ) : gr(g), post(p) {}
  virtual void operator()( wfa::ITrans* t ) 
  {
    //t->print(std::cout << "\n*********************\n  +++SetSource: ");
    //std::cout << "\n*********************\n";
    if(!post) {
      gr.setSource(Transition(*t),t->weight());
    } else {
      //ETrans --> Esource
      LazyTrans *lt = static_cast<LazyTrans *> (t);
      ETrans* etrans = lt->getETrans();
      if (0 != etrans) {
        gr.setESource(Transition(*t), etrans->getWeightAtCall(), etrans->weight());
      } else {
        gr.setSource(Transition(*t), t->weight());
      }
    }

  }
};

struct FWPDSCompareFunctor : public wfa::ConstTransFunctor
{
  graph::InterGraph & gr;
  bool iseq;
  FWPDSCompareFunctor( graph::InterGraph & p ) : gr(p) {
    iseq = true; 
  }

  virtual void operator()( const wfa::ITrans* t ) {
    sem_elem_t wt1 = gr.get_weight(Transition(t->from(), t->stack(), t->to()));
    sem_elem_t wt2 = t->weight();
    if( !wt2->equal(wt1) ) {
      iseq = false;
      wt1->print(*waliErr) << "\n";
      t->print(*waliErr) << "\n";
      *waliErr << "-----------------------------\n";
    }
    const ETrans *et = dynamic_cast<const ETrans*>(t);
    if(et != 0) {
      wt1 = et->getWeightAtCall();
      wt2 = gr.get_call_weight(Transition(t->from(), t->stack(), t->to()));
      if( !wt2->equal(wt1) ) {
        iseq = false;
        wt1->print(*waliErr) << "\n";
        t->print(*waliErr) << "\n";
        *waliErr << "-----------------------------\n";
      }
    }

  }

  std::ostream& print( std::ostream& out ) {
    if(!iseq) {
      out << "Not equal\n";
      assert(0);
    }
    return out;
  }
};

std::ostream& graphPrintKey( int k, std::ostream& o ) {
  return wali::printKey(o,(Key)k);
}

void FWPDS::prestar( wfa::WFA const & input, wfa::WFA& output )
{
  // setup output
  addEtrans = true;
  EWPDS::prestarSetupFixpoint(input,output);
  addEtrans = false;

  // If theZero is invalid, then there
  // are no rules and not saturation to 
  // be done.
  if (!theZero.is_valid()) {
    return;
  }

  // cache semiring 1
  wghtOne = theZero->one();

  // FIXME: Currently FWPDS always assumes that the
  // underlying pds is a EWPDS. In the absence of
  // merge functions, it can be treated as a WPDS.
  // However, there is no cost benefit in using WPDS
  // (it only saves on debugging effort)
  interGr = new graph::InterGraph(theZero, true, true);

  // Input transitions become source nodes in FWPDS
  FWPDSSourceFunctor sources(*interGr.get_ptr(), false);
  output.for_each(sources);

  // Build the InterGraph using EWPDS saturation without weights
  EWPDS::prestarComputeFixpoint(output);

  // Compute summaries
  interGr->setupInterSolution();

  //interGr->print(std::cout << "THE INTERGRAPH\n",graphPrintKey);

  // Copy information back from InterGraph to the
  // output WFA. This does not do computation on weights,
  // but instead uses LazyTrans to put in "lazy" weights
  // that are evaluated on demand.
  FWPDSCopyBackFunctor copier( interGr );
  output.for_each(copier);


  checkResults(input,false);

  interGr = NULL;
  currentOutputWFA = 0;
}

void FWPDS::prestar_handle_call(wfa::ITrans *t1,
    wfa::ITrans *t2,
    rule_t &r,
    sem_elem_t delta 
    )
{
  if(checkingPhase) {
    return EWPDS::prestar_handle_call(t1, t2, r, delta);
  }

  LazyTrans *lt1 = static_cast<LazyTrans *>(t1);
  LazyTrans *lt2 = static_cast<LazyTrans *>(t2);

  ETrans *et1 = lt1->getETrans();
  ETrans *et2 = lt2->getETrans();

  assert(!(et1 == 0 && et2 != 0));

  if(et1 != 0) {
    ERule *er = (ERule *)(r.get_ptr());
    interGr->addEdge(Transition(*t2),
                     Transition(*t1),
                     Transition(r->from()->state(), r->from()->stack(),t2->to()),
                     er->merge_fn().get_ptr() );
  } else {
    interGr->addEdge(Transition(*t2),
                     Transition(*t1),
                     Transition(r->from()->state(), r->from()->stack(),t2->to()),
                     r->weight() );    
  }

  // update
  if(et2 != 0) {
    update_etrans( r->from()->state(), r->from()->stack(), t2->to(),
                   wghtOne,r->from() );
  } else {
    update( r->from()->state(), r->from()->stack(), t2->to(),
            wghtOne,r->from() );
  }

}

void FWPDS::prestar_handle_trans(
    wfa::ITrans * t ,
    wfa::WFA & fa   ,
    rule_t & r,
    sem_elem_t delta 
    )
{
  //std::cout << "Prestar_handle_trans:\n";
  //t->print(std::cout << "t: ") << "\n";
  //r->print(std::cout << "r: ") << "\n";

  if(checkingPhase)
  {
    return EWPDS::prestar_handle_trans(t, fa, r, delta);
  }


  Key fstate = r->from()->state();
  Key fstack = r->from()->stack();

  if( r->is_rule2() ) {
    KeyPair kp( t->to(),r->stack2() );
    wfa::WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
    wfa::WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
    wfa::TransSet::iterator tsit;

    if(kpit != kpitEND)
    {
      wfa::TransSet & transSet = kpit->second;
      for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
      {
        wfa::ITrans* tprime = *tsit;
        prestar_handle_call(t, tprime, r, delta);
      }
    }
  }
  else {
    LazyTrans *lt = static_cast<LazyTrans *> (t);
    ETrans *et = lt->getETrans();

    interGr->addEdge(Transition(*t),
        Transition(fstate,fstack,t->to()),
        r->weight());
    if(et != 0) {
      update_etrans( fstate, fstack, t->to(), wghtOne, r->from() );
    } else {
      update( fstate, fstack, t->to(), wghtOne, r->from() );
    }

  }
}


bool FWPDS::checkResults( wfa::WFA const & input, bool do_poststar )
{
  if( wali::get_verify_fwpds() ) 
  {

    // set flag to use EWPDS's saturation
    checkingPhase = true;

    wfa::WFA tmpOutput;
    wfa::WFA tmpInput(input);

    util::Timer* wpdsTimer = new util::Timer("\t[WPDS] Total Timer ");
    if( do_poststar ) {
      EWPDS::poststar(tmpInput, tmpOutput);
      //WPDS::poststarSetupFixpoint(tmpInput,tmpOutput);
      //WPDS::poststarComputeFixpoint(tmpOutput);
    }
    else {
      EWPDS::prestar(tmpInput, tmpOutput);
      //WPDS::prestarSetupFixpoint(tmpInput,tmpOutput);
      //WPDS::prestarComputeFixpoint(tmpOutput);
    }
    delete wpdsTimer;

    { // compare results
      util::Timer timer("\t[(F)WPDS] Compare Results ");
      FWPDSCompareFunctor comp(*interGr);
      //output.print( std::cout << "\nFWPDS OUTPUT:\n\n" );
      //tmpOutput.print( std::cout << "\nWPDS OUTPUT:\n\n" );
      tmpOutput.for_each(comp);
      comp.print(std::cout);
    }
    *waliErr << "\t[(F)WPDS] CONGRATS DUDE...OUTPUT MATCHED.\n";

    checkingPhase = false;
  }
  return true;
}

void FWPDS::poststar( wfa::WFA const & input, wfa::WFA& output ) {
  poststarIGR(input, output);
  
  //{
  //  util::Timer timer("FWPDS All Weights");
  //  interGr->update_all_weights();
  //}
  //interGr->print_stats(*waliErr) << "\n";

  interGr = NULL;
}

void FWPDS::poststarIGR( wfa::WFA const & input, wfa::WFA& output )
{

  EWPDS::poststarSetupFixpoint(input,output);

  // If theZero is invalid then no rules have
  // been added to the WPDS and no saturation
  // can be done.
  if(!theZero.is_valid()) {
    return;
  }
  // cache semiring 1
  wghtOne = theZero->one();

  // FIXME: Currently FWPDS always assumes that the
  // underlying pds is a EWPDS. In the absence of
  // merge functions, it can be treated as a WPDS.
  // However, there is no cost benefit in using WPDS
  interGr = new graph::InterGraph(theZero, true, false);

  // Input transitions become source nodes in FWPDS
  FWPDSSourceFunctor sources(*interGr.get_ptr(), true);
  output.for_each(sources);

  // Build the InterGraph using EWPDS saturation without weights
  EWPDS::poststarComputeFixpoint(output);

  {
    std::string msg = (get_verify_fwpds()) ? "FWPDS Saturation" : "";
    util::Timer timer(msg);
    // Compute summaries
    interGr->setupInterSolution();
  }

  //interGr->print(std::cout << "THE INTERGRAPH\n",graphPrintKey);

  // Copy information back from InterGraph to the
  // output WFA. This does not do computation on weights,
  // but instead uses LazyTrans to put in "lazy" weights
  // that are evaluated on demand.
  FWPDSCopyBackFunctor copier( interGr );
  output.for_each(copier);

  checkResults(input,true);

  currentOutputWFA = 0;
}

void FWPDS::poststar_handle_eps_trans(wfa::ITrans* teps, wfa::ITrans* tprime, sem_elem_t delta) 
{
  if(checkingPhase) 
  {
    return EWPDS::poststar_handle_eps_trans(teps, tprime, delta);
  }

  // These casts should never fail. However, if WALi evolves
  // to allow for more types of transitions, then it is nice
  // to leave this here.
#if FWPDS_DYN_CAST
  LazyTrans* lt = dynamic_cast<LazyTrans*>(tprime);
  { // <DEBUG>
    if (0 == lt) {
      *waliErr << "[ERROR] Oops, FWPDS is broken.\n";
      *waliErr << "    >> Expeced LazyTrans, Got >> ";
      tprime->print(*waliErr);
      assert(0);
    }
  } // </DEBUG>
#else
  LazyTrans* lt = static_cast<LazyTrans*>(tprime);
#endif
  ewpds::ETrans* etrans = lt->getETrans();
  if (0 != etrans) {
    erule_t r = etrans->getERule();

    interGr->addEdge(Transition(*tprime),
        Transition(*teps),
        Transition(teps->from(),tprime->stack(),tprime->to()),
        etrans->getMergeFn());

  } else {
    interGr->addEdge(Transition(*tprime),
        Transition(*teps),
        Transition(teps->from(),tprime->stack(),tprime->to()),
        wghtOne);
  }

  Config * config = make_config( teps->from(),tprime->stack() );
  update( teps->from()
      , tprime->stack()
      , tprime->to()
      , wghtOne
      , config
      );
}

void FWPDS::poststar_handle_trans(
    wfa::ITrans* t, wfa::WFA &fa, 
    rule_t &r, sem_elem_t delta)
{
  if(checkingPhase) 
  {
    return EWPDS::poststar_handle_trans(t, fa, r, delta);
  }

  Key rtstate = r->to_state();
  Key rtstack = r->to_stack1();

  if( r->to_stack2() == WALI_EPSILON ) {
    update( rtstate, rtstack, t->to(), wghtOne, r->to() );
    interGr->addEdge(Transition(*t),
        Transition(rtstate,rtstack,t->to()),
        r->weight());
  }
  else {  // Push rule (p,g) -> (p,g',g2)

    // Is a rule 2 so we must generate a state
    // and create 2 new transitions
    Key gstate = gen_state( rtstate,rtstack );
    // Note: QuasiOne is not supported in FWPDS

    wfa::ITrans* tprime = update_prime( gstate, t, r, delta, wghtOne);
    update( rtstate, rtstack, gstate, wghtOne, r->to() );

    // add source edge
    interGr->setSource(Transition(rtstate,rtstack, gstate), wghtOne);
    // add edge (p,g,q) -> (p,g',(p,g'))
    interGr->addCallEdge(Transition(*t),Transition(rtstate,rtstack, gstate));
    // add call-ret edge (p,g,q) -> ((p,g'),rstk2,q)
    interGr->addCallRetEdge(Transition(*t),
        Transition(gstate, r->to_stack2(),t->to()),
        r->weight());

    if( tprime->modified() )
    {

      wfa::WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->from() );
      if( epsit != fa.eps_map.end() )
      {
        // get epsilon list ref
        wfa::TransSet& transSet = epsit->second;
        // iterate
        wfa::TransSet::iterator tsit = transSet.begin();
        for( ; tsit != transSet.end() ; tsit++ )
        {
          wfa::ITrans * teps = *tsit;
          poststar_handle_eps_trans(teps, tprime, delta);

        }
      }
    }
  }
}



////////////////////////////////////////////
// These guys take care of LazyTrans stuff
////////////////////////////////////////////

void FWPDS::update(
    Key from,
    Key stack,
    Key to,
    sem_elem_t se,
    Config * cfg )
{
  if(checkingPhase) {
    return EWPDS::update(from, stack, to, se, cfg);
  }

  wfa::ITrans *t;
  if(addEtrans) {
    t = new ETrans(from, stack, to, 0, se, 0);
  } else {
    t = new wfa::Trans(from, stack, to, se);
  }
  t->setConfig(cfg);

  LazyTrans * lt = new LazyTrans(t);
  t = currentOutputWFA->insert(lt);

  if( t->modified() ) {
    //t->print(std::cout << "Adding transition: ") << "\n";
    worklist->put( t );
  }
}

wfa::ITrans* FWPDS::update_prime(
    Key from, //<! Guaranteed to be a generated state
    wfa::ITrans* call, //<! The call transition
    rule_t r, //<! The push rule
    sem_elem_t delta, //<! Delta change on the call transition
    sem_elem_t wWithRule //<! delta \extends r->weight()
    )
{
  if(checkingPhase) {
    return EWPDS::update_prime(from, call, r, delta, wWithRule);
  }

  //
  // !!NOTE!!
  // This code is copied from EWPDS::update_prime.
  // Changes here should be reflected there.
  //
  ERule* er = (ERule*)r.get_ptr();
  wfa::ITrans* et = 
    new ETrans(
        from, r->to_stack2(), call->to(),
        delta, wWithRule, er);
  LazyTrans* lt = new LazyTrans(et);
  wfa::ITrans* t = currentOutputWFA->insert(lt);
  return t;
}

void FWPDS::operator()( wfa::ITrans const * orig ) {
  if(checkingPhase) {
    return EWPDS::operator()(orig);
  }

  if( is_strict() && is_pds_state(orig->to())) {
    *waliErr << "WALi Error: cannot have incoming transition to a PDS state\n";
    orig->print( *waliErr << "    " ) << std::endl;
    assert(0);
  }

  Config *c = make_config( orig->from(),orig->stack() );
  sem_elem_t se = 
    (wrapper.is_valid()) ? wrapper->wrap(*orig) : orig->weight();

  LazyTrans *t = new LazyTrans( orig->copy() );

  t->setConfig(c);
  t->setWeight(se);

  // fa.addTrans takes ownership of passed in pointer
  currentOutputWFA->addTrans( t );

  // add t to the worklist for saturation
  worklist->put( t );
}


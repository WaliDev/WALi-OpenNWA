/*!
 * @author Nick Kidd
 * @author Akash Lal
 *
 * $Id$
 */

#include "wali/Worklist.hpp"
#include "wali/Common.hpp"
#include "wali/HashMap.hpp"
#include "wali/SemElem.hpp"
#include "wali/KeyPairSource.hpp"
#include "wali/wpds/GenKeySource.hpp"

#include "wali/util/Timer.hpp"

#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/TransFunctor.hpp"

#include "wali/wpds/Rule.hpp"
#include "wali/wpds/ewpds/ERule.hpp"
#include "wali/wpds/Config.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"

#include "wali/graph/RegExp.hpp"
#include "wali/graph/InterGraph.hpp"
#include "wali/wpds/fwpds/LazyTrans.hpp"

using namespace wali;
using namespace wali::graph;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::ewpds;
using wfa::Trans;
using wfa::TransSet;
using wfa::WFA;
using wfa::State;

FWPDS::FWPDS() : EWPDS(), interGr(NULL), checkingPhase(false)
{
}

FWPDS::FWPDS(wali::wpds::Wrapper* wrapper) : EWPDS(wrapper) , interGr(NULL), checkingPhase(false)
{
}

///////////////////////////////////////////////////////////////////
void FWPDS::topDownEval(bool f) {
  wali::graph::RegExp::topDownEval(f);
}

struct FWPDSCopyBackFunctor : public wali::wfa::TransFunctor
{
    ref_ptr<wali::graph::InterGraph> gr;
    FWPDSCopyBackFunctor(ref_ptr<wali::graph::InterGraph> _gr) : gr(_gr) {}
    virtual void operator()( wfa::Trans* t ) {
      LazyTrans *lt = static_cast<LazyTrans *> (t);
      if (::wali::is_lazy_fwpds())
          lt->setInterGraph(gr);
      else
          lt->setWeight( gr->get_weight( Transition(*t) ) );
    }
};

struct FWPDSSourceFunctor : public wali::wfa::ConstTransFunctor
{
    wali::graph::InterGraph & gr;
    FWPDSSourceFunctor( wali::graph::InterGraph & p ) : gr(p) {}
    virtual void operator()( const wfa::Trans* t ) 
    {
        //t->print(std::cout << "\n*********************\n  +++SetSource: ");
        //std::cout << "\n*********************\n";
        gr.setSource(Transition(*t),t->weight());
    }
};

struct FWPDSCompareFunctor : public wali::wfa::ConstTransFunctor
{
    wali::graph::InterGraph & gr;
    bool iseq;
    FWPDSCompareFunctor( wali::graph::InterGraph & p ) : gr(p) {
        iseq = true; 
    }

    virtual void operator()( const wfa::Trans* t ) {
        sem_elem_t wt1 = gr.get_weight(Transition(t->from(), t->stack(), t->to()));
        sem_elem_t wt2 = t->weight();
        if( !wt2->equal(wt1) ) {
            iseq = false;
            wt1->print(*waliErr) << "\n";
            t->print(*waliErr) << "\n";
            *waliErr << "-----------------------------\n";
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

void
FWPDS::prestar( wfa::WFA& input, wfa::WFA& output )
{
    // Get hold of semiring element
    sem_elem_t se = input.getSomeWeight();

    // if se is NULL then input is empty
    if(!se.is_valid()) {
      // Get weight from rule0s
      for(std::set<Config*>::iterator it = WPDS::rule_zeroes.begin();
          it != rule_zeroes.end() && !se.is_valid(); it++) {
        Config *cloc = *it;
        Config::reverse_iterator rit = cloc->rbegin();
        for( ; rit != cloc->rend(); rit++) {
          rule_t r = *rit;
          se = r->weight();
          break;
        }
      }
      if(!se.is_valid()) {
        output.clear();
        return;
      }
    }
    // cache semiring 1
    wghtOne = se->one();

    // FIXME: Currently FWPDS always assumes that the
    // underlying pds is a EWPDS. In the absence of
    // merge functions, it can be treated as a WPDS.
    // However, there is no cost benefit in using WPDS
    // (it only saves on debugging effort)
    interGr = new wali::graph::InterGraph(se, true, true);

    // setup output
    EWPDS::prestarSetupFixpoint(input,output);

    // Input transitions become source nodes in FWPDS
    FWPDSSourceFunctor sources(*interGr);
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

void FWPDS::prestar_handle_call(Trans *t1,
                                Trans *t2,
                                rule_t &r,
                                sem_elem_t delta 
                                )
{
  if(checkingPhase) {
    return EWPDS::prestar_handle_call(t1, t2, r, delta);
  }

  if(!is_pds_state(t2->from())) {
    interGr->addEdge(Transition(*t2),
                     Transition(*t1),
                     Transition(r->from()->state(), r->from()->stack(),t2->to()),
                     r->weight() );    
  } else { // apply merge function
    ERule *er = (ERule *)(r.get_ptr());
    interGr->addEdge(Transition(*t2),
                     Transition(*t1),
                     Transition(r->from()->state(), r->from()->stack(),t2->to()),
                     er->merge_fn().get_ptr() );
  }
  // update
  update( r->from()->state(), r->from()->stack(), t2->to(),
          wghtOne,r->from() );

}

void FWPDS::prestar_handle_trans(
        Trans * t ,
        WFA & fa   ,
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
    WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
    WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
    TransSet::iterator tsit;
    
    if(kpit != kpitEND)
      {
        TransSet & transSet = kpit->second;
        for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
          {
            Trans * tprime = *tsit;
            prestar_handle_call(t, tprime, r, delta);
          }
      }
  }
  else {
    interGr->addEdge(Transition(*t),
                     Transition(fstate,fstack,t->to()),
                     r->weight());
    update( fstate, fstack, t->to(), wghtOne, r->from() );
  }
}


bool
FWPDS::checkResults( wfa::WFA& input, bool poststar )
{
    if( wali::get_verify_fwpds() ) 
    {

        // set flag to use EWPDS's saturation
        checkingPhase = true;

        wfa::WFA tmpOutput;
        wfa::WFA tmpInput(input);

        util::Timer* wpdsTimer = new util::Timer("\t[WPDS] Total Timer ");
        if( poststar ) {
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

void
FWPDS::poststar( wfa::WFA& input, wfa::WFA& output )
{
    // Get hold of semiring element
    sem_elem_t se = input.getSomeWeight();

    // if se is NULL then input is empty
    if(!se.is_valid()) {
      output.clear();
      return;
    }
    // cache semiring 1
    wghtOne = se->one();

    // FIXME: Currently FWPDS always assumes that the
    // underlying pds is a EWPDS. In the absence of
    // merge functions, it can be treated as a WPDS.
    // However, there is no cost benefit in using WPDS
    interGr = new wali::graph::InterGraph(se, true, false);

    EWPDS::poststarSetupFixpoint(input,output);

    // Input transitions become source nodes in FWPDS
    FWPDSSourceFunctor sources(*interGr);
    output.for_each(sources);

    // Build the InterGraph using EWPDS saturation without weights
    EWPDS::poststarComputeFixpoint(output);

    // Compute summaries
    interGr->setupInterSolution();

    //interGr->print(std::cout << "THE INTERGRAPH\n",graphPrintKey);

    // Copy information back from InterGraph to the
    // output WFA. This does not do computation on weights,
    // but instead uses LazyTrans to put in "lazy" weights
    // that are evaluated on demand.
    FWPDSCopyBackFunctor copier( interGr );
    output.for_each(copier);


    checkResults(input,true);

    interGr = NULL;
    currentOutputWFA = 0;
}

void FWPDS::poststar_handle_eps_trans(Trans *teps, Trans *tprime, sem_elem_t delta) 
{
    if(checkingPhase) 
    {
        return EWPDS::poststar_handle_eps_trans(teps, tprime, delta);
    }
    
    KeySource *ks = getKeySource(teps->to());
    GenKeySource* gks = dynamic_cast<GenKeySource*>(ks);
    KeyPairSource *kps;
    if (gks != 0) { // check gks first b/c that is what poststar generates now
      kps = dynamic_cast<KeyPairSource*>(getKeySource(gks->getKey()));
    }
    else {
      // this probably should not occur, unless the user
      // created teps himself.
      kps = dynamic_cast<KeyPairSource*>(ks);
    }

    sem_elem_t wght;
                
    if(0 != kps) { // apply merge function
      // Find the rule first
      rule_t r = EWPDS::lookup_rule(kps->get_key_pair().first, kps->get_key_pair().second, tprime->stack());
      assert(r.get_ptr() != NULL);

      ERule *er = (ERule *)(r.get_ptr());

      interGr->addEdge(Transition(r->from_state(), r->from_stack(), tprime->to()),
                       Transition(*teps),
                       Transition(teps->from(),tprime->stack(),tprime->to()),
                       er->merge_fn().get_ptr());

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
        Trans *t, WFA &fa, 
        rule_t &r, sem_elem_t delta)
{
    if(checkingPhase) 
    {
      return EWPDS::poststar_handle_trans(t, fa, r, delta);
    }

    ERule *er = (ERule *)(r.get_ptr());
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
      
      Trans * tprime = update_prime( gstate, r->to_stack2(), t->to(), wghtOne  );
      update( rtstate, rtstack, gstate, wghtOne, r->to() );

      // add source edge
      interGr->setSource(Transition(rtstate,rtstack, gstate), wghtOne);
      // add edge (p,g,q) -> (p,g',(p,g'))
      interGr->addCallEdge(Transition(*t),Transition(rtstate,rtstack, gstate));
      // add edge (p,g,q) -> ((p,g'),rstk2,q)
      interGr->addEdge(Transition(*t),
                       Transition(gstate, r->to_stack2(),t->to_state()),
                       r->weight());
      
      if( tprime->modified() )
        {
          
          WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->from() );
          if( epsit != fa.eps_map.end() )
            {
              // get epsilon list ref
              TransSet& transSet = epsit->second;
              // iterate
              TransSet::iterator tsit = transSet.begin();
              for( ; tsit != transSet.end() ; tsit++ )
                {
                  Trans * teps = *tsit;

                  interGr->addEdge(Transition(*t),
                                   Transition(*teps),
                                   Transition(teps->from(),tprime->stack(),tprime->to()),
                                   er->merge_fn().get_ptr());

                  Config * config = make_config( teps->from(),tprime->stack() );
                  
                  update( teps->from(),tprime->stack(),tprime->to(),
                          wghtOne, config );
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
  LazyTrans * lt = new LazyTrans(from,stack,to,se,cfg);
  Trans *t = currentOutputWFA->insert(lt);
  if( t->modified() ) {
      //t->print(std::cout << "Adding transition: ") << "\n";
      worklist->put( t );
  }
}

wfa::Trans * FWPDS::update_prime(
        Key from,
        Key stack,
        Key to,
        sem_elem_t se )
{
  LazyTrans * t = new LazyTrans(from,stack,to,se,0);
  Trans * tmp = currentOutputWFA->insert(t);
  return tmp;
}

void FWPDS::operator()( wali::wfa::Trans * orig ) {
  if(checkingPhase) {
    return EWPDS::operator()(orig);
  }

  Config *c = make_config( orig->from(),orig->stack() );
  sem_elem_t se = 
      (wrapper == 0) ? orig->weight() : wrapper->wrap(*orig);

  LazyTrans *t = new LazyTrans( orig->from()
                     ,orig->stack()
                     ,orig->to()
                     ,se
                     ,c);
  
  // fa.addTrans takes ownership of passed in pointer
  currentOutputWFA->addTrans( t );

  // add t to the worklist for saturation
  worklist->put( t );
}


/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */

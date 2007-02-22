/*!
 * @author Nick Kidd
 */

#include "wali/Worklist.hpp"
#include "wali/Common.hpp"
#include "wali/HashMap.hpp"
#include "wali/SemElem.hpp"

#include "wali/util/Timer.hpp"

#include "wali/wfa/WFA.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/TransFunctor.hpp"

#include "wali/wpds/Rule.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/LinkedTrans.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"

//#include "wali/wpds/fwpds/InterGraph.hpp"
#include "wali/graph/InterGraph.hpp"

using namespace wali;
using namespace wali::graph;
using namespace wali::wpds::fwpds;

FWPDS::FWPDS() : WPDS()
{
}

FWPDS::FWPDS(Wrapper* wrapper) : WPDS(wrapper) 
{
}

FWPDS::FWPDS( Worklist<wfa::Trans> * worklist ) : WPDS(worklist)
{
}

FWPDS::FWPDS( Wrapper * wrapper , Worklist<wfa::Trans> * worklist ) :
    WPDS(wrapper,worklist)
{
}

struct FWPDSCopyBackFunctor : public wali::wfa::TransFunctor
{
    wali::graph::InterGraph& gr;
    FWPDSCopyBackFunctor(wali::graph::InterGraph& gr) : gr(gr) {}
    virtual void operator()( wfa::Trans* t ) {
        t->setWeight( gr.get_weight( Transition(*t) ) );
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
    util::Timer* fwpdsTimer = new util::Timer("\t[FWPDS] Total Time ");
    wali::graph::InterGraph* gr = prestarComputeInterGraph(input,output);
    assert(gr != 0);
    FWPDSCopyBackFunctor copier( *gr );
    {
        util::Timer t("\t[FWPDS] Solve all weights ");
        output.for_each(copier);
    }
    delete fwpdsTimer;
    checkResults(input,gr,false);
    delete gr;
}

wali::graph::InterGraph*
FWPDS::prestarComputeInterGraph( wfa::WFA& input, wfa::WFA& output )
{
    this->prestarSetupFixpoint(input,output);
    LinkedTrans *t = 0;
    InterGraph* gr = 0;
    if( get_from_worklist(t) ) {
        sem_elem_t se = t->weight();
        gr = new wali::graph::InterGraph(se,false,true);

        FWPDSSourceFunctor sources(*gr);
        output.for_each(sources);

        { // reachability
            util::Timer timer("\tFWPDS reachability");
            do {
                // NAK DEBUG
                //t->print( std::cout << "Popped t => " ) << "\n";
                pre(t,output, *gr);
            } while( get_from_worklist(t) );
        }

        gr->setupInterSolution();
        // THIS computes all weights
        //gr->update_all_weights();

        // This prints a lot
        //gr->print(std::cout << "THE INTERGRAPH\n",graphPrintKey);

    }
    return gr;
}

void 
FWPDS::pre(LinkedTrans* t, ::wali::wfa::WFA& fa, wali::graph::InterGraph& gr )
{
    // Get config
    Config * config = t->config;

    { // BEGIN DEBUGGING
        assert( config );
    } // END DEBUGGING

    sem_elem_t wghtOne = t->weight()->one();
    //sem_elem_t dnew = t->delta;
    //t->delta = dnew->zero();
    t->setDelta(wghtOne->zero());

    // For each backward rule of config
    Config::reverse_iterator bwit = config->rbegin();
    for( ; bwit != config->rend() ; bwit++ )
    {
        rule_t & r = *bwit;

        //prestar_handle_trans( t,fa,r,dnew );
        // inlined prestar_handle_trans
        {

            //sem_elem_t wrule_trans = r->weight()->extend( delta );
            Key fstate = r->from()->state();
            Key fstack = r->from()->stack();

            if( r->is_rule2() )
            {
                KeyPair kp( t->to(),r->stack2() );
                { // BEGIN DEBUGGING
                    //r->print( *waliErr << "Handling Rule 2: " ) << std::endl;
                    //*waliErr << "\t(" << key2str(kp.first) << ", " << key2str(kp.second) << ",*)\n";
                } // END DEBUGGING
                wali::wfa::WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
                wali::wfa::WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
                wali::wfa::TransSet::iterator tsit;

                if( kpit != kpitEND )
                {
                    wali::wfa::TransSet& transSet = kpit->second;
                    for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
                    {
                        wali::wfa::Trans * tprime = *tsit;
                        { // BEGIN DEBUGGING
                            //*waliErr << "\tMatched: (" << key2str(tprime->from()) << ", ";
                            //*waliErr << key2str(tprime->stack()) << ", ";
                            //*waliErr << key2str(tprime->to()) << ")\n";
                        } // END DEBUGGING
                        //sem_elem_t wtp = wrule_trans->extend( tprime->weight() );
                        update( fstate
                                , fstack
                                , tprime->to()
                                , wghtOne
                                , r->from()
                              );
                        gr.addEdge(Transition(*tprime),
                                Transition(*t),
                                Transition(fstate,fstack,tprime->to()),
                                r->weight());
                    }
                }
            }
            else {
                update( fstate
                        , fstack
                        , t->to()
                        , wghtOne
                        , r->from() 
                      );
                gr.addEdge(Transition(*t),
                        Transition(fstate,fstack,t->to()),
                        r->weight());
            }
        }
    }

    // check matching rule 2s 
    r2hash_t::iterator r2it = r2hash.find( t->stack() );

    // does a rule 2 exist with matching second symbol on rhs
    if( r2it != r2hash.end() )
    {
        // get reference
        std::list< rule_t > & ls = r2it->second;
        std::list< rule_t >::iterator lsit;

        // loop through list
        for( lsit = ls.begin() ; lsit != ls.end() ; lsit++ )
        {
            rule_t & r = *lsit;

            wali::wfa::Trans tp;
            if( fa.find(r->to_state(),r->to_stack1(),t->from(),tp) )
            {
                // f(r) * t'
                //sem_elem_t wrtp = r->weight()->extend( tp.weight() );

                // f(r) * t' * delta
                //sem_elem_t wnew = wrtp->extend( dnew );

                // update
                Key fstate = r->from()->state();
                Key fstack = r->from()->stack();
                update( fstate
                        , fstack
                        , t->to()
                        , wghtOne
                        , r->from()
                      );
                gr.addEdge(Transition(*t),
                        Transition(tp),
                        Transition(fstate,fstack,t->to()),
                        r->weight());

            }
        }
    }
}

void
FWPDS::prestarComputeFixpoint( wfa::WFA& fa )
{
    *waliErr << "[ERROR] FWPDS::prestarComputeFixpoint called?\n";
    assert(0);
}

bool
FWPDS::checkResults( wfa::WFA& input, wali::graph::InterGraph* gr,bool poststar )
{
    if( wali::get_verify_fwpds() ) 
    {
        wfa::WFA tmpOutput;
        wfa::WFA tmpInput(input);
        // run post*
        util::Timer* wpdsTimer = new util::Timer("\t[WPDS] Total Timer ");
        if( poststar ) {
            WPDS::poststarSetupFixpoint(tmpInput,tmpOutput);
            WPDS::poststarComputeFixpoint(tmpOutput);
        }
        else {
            WPDS::prestarSetupFixpoint(tmpInput,tmpOutput);
            WPDS::prestarComputeFixpoint(tmpOutput);
        }
        delete wpdsTimer;

        { // compare results
            util::Timer timer("\t[(F)WPDS] Compare Results ");
            FWPDSCompareFunctor comp(*gr);
            //output.print( std::cout << "\nFWPDS OUTPUT:\n\n" );
            //tmpOutput.print( std::cout << "\nWPDS OUTPUT:\n\n" );
            tmpOutput.for_each(comp);
            comp.print(std::cout);
        }
    }
    *waliErr << "\t[(F)WPDS] CONGRATS DUDE...OUTPUT MATCHED.\n";
    return true;
}

void
FWPDS::poststar( wfa::WFA& input, wfa::WFA& output )
{
    util::Timer* fwpdsTimer = new util::Timer("\t[FWPDS] Total Time ");
    wali::graph::InterGraph* gr = poststarComputeInterGraph(input,output);
    assert( gr != 0 );
    FWPDSCopyBackFunctor copier( *gr );
    {
        util::Timer t("\t[FWPDS] Solve all weights ");
        output.for_each(copier);
    }
    checkResults(input,gr,true);
    delete fwpdsTimer;

    delete gr;
}

wali::graph::InterGraph*
FWPDS::poststarComputeInterGraph( wfa::WFA& input, wfa::WFA& output )
{
    this->poststarSetupFixpoint(input,output);
    LinkedTrans *t = 0;
    InterGraph* gr = 0;
    if( get_from_worklist(t) ) {
        sem_elem_t se = t->weight();
        gr = new wali::graph::InterGraph(se,false,false);

        FWPDSSourceFunctor sources(*gr);
        output.for_each(sources);

        { // reachability
            util::Timer timer("\tFWPDS reachability");
            do {
                // NAK DEBUG
                //t->print( std::cout << "Popped t => " ) << "\n";
                post(t,output, *gr);
            } while( get_from_worklist(t) );
        }

        gr->setupInterSolution();
        // THIS computes all weights
        //gr->update_all_weights();

        // This prints a lot
        //gr->print(std::cout << "THE INTERGRAPH\n",graphPrintKey);

    }
    return gr;
}

void
FWPDS::poststarComputeFixpoint( wfa::WFA& fa )
{
    *waliErr << "[ERROR] FWPDS::poststarComputeFixpoint called?\n";
    assert(0);
}

void
FWPDS::post(LinkedTrans* t, wfa::WFA& fa, InterGraph& gr )
{
    // Get config
    wpds::Config * config = t->config;

    // Get delta
    //sem_elem_t dnew = t->delta;
    // FWPDS just uses 1
    sem_elem_t wghtOne = t->weight()->one();

    // Reset delta of t to zero to signify completion
    // of work for that delta
    //
    // WALi uses the delta to check for modification so 
    // this step is important
    t->setDelta(wghtOne->zero());

    // For each forward rule of config
    // Apply rule to create new transition
    if( WALI_EPSILON != t->stack() )
    {
        Config::iterator fwit = config->begin();
        for( ; fwit != config->end() ; fwit++ ) {
            rule_t & r = *fwit;
            // NAK DEBUG
            //r->print( std::cout << "\tMatched r => " ) << "\n";

            // Step rule
            if( r->to_stack2() == WALI_EPSILON ) {
                update(r->to_state(), r->to_stack1(),t->to(),wghtOne,r->to() );
                gr.addEdge(Transition(*t),
                        Transition(r->to_state(),r->to_stack1(),t->to()),
                        r->weight());
            }
            // Push rule (p,g) -> (p,g',g2)
            else {
                // TODO quasi one?
                Key gstate = gen_state(r->to_state(),r->to_stack1());
                // TODO Is the weight right?
                wfa::Trans * tprime = update_prime( gstate, r->to_stack2(), t->to(), wghtOne );
                wfa::State* state = fa.getState(gstate);

                // add (p,g',(p,g'))
                update( r->to_state(), r->to_stack1(), gstate, wghtOne, r->to() );
                // add source
                gr.setSource(Transition(r->to_state(),r->to_stack1(), gstate), wghtOne);
                // add edge (p,g,q) -> (p,g',(p,g'))
                gr.addCallEdge(Transition(*t),Transition(r->to_state(),r->to_stack1(), gstate));
                // add edge (p,g,q) -> ((p,g'),rstk2,q)
                gr.addEdge(Transition(*t),
                        Transition(gstate, r->to_stack2(),t->to_state()),
                        r->weight());

                if( tprime->modified() ) {

                    wfa::WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->from() );
                    if( epsit != fa.eps_map.end() )
                    {
                        // tprime stack key
                        Key tpstk = tprime->stack();
                        //tprime to key
                        Key tpto = tprime->to();
                        // get epsilon list ref 
                        wfa::TransSet& transSet = epsit->second;
                        //iterate 
                        wfa::TransSet::iterator tsit = transSet.begin();
                        for( ; tsit != transSet.end() ; tsit++ ) { 
                            wfa::Trans * teps = *tsit; 
                            wpds::Config * config = make_config( teps->from(),tpstk );
                            //sem_elem_t epsW = tprime->getDelta()->extend( teps->weight() );
                            update(teps->from(),tpstk, tpto, /*epsW*/wghtOne, config );
                            gr.addEdge(Transition(*tprime),
                                        Transition(*teps),
                                        Transition(teps->from(), tprime->stack(), tprime->to()),
                                        wghtOne);

                        }
                    }
                }
            }
        }
    }
    else {
        // (p,eps,q) + (q,y,q') => (p,y,q')
        wfa::State * state = fa.getState( t->to() );
        wfa::State::iterator it = state->begin();
        for(  ; it != state->end() ; it++ )
        {
            wfa::Trans * tprime = *it;
            //sem_elem_t wght = tprime->weight()->extend( dnew );
            wpds::Config * config = make_config( t->from(),tprime->stack() );
            update( t->from()
                    , tprime->stack()
                    , tprime->to()
                    , wghtOne
                    , config
                  );
            gr.addEdge(Transition(*tprime),
                    Transition(*t),
                    Transition(t->from(),tprime->stack(),tprime->to()),
                    wghtOne);

        }
    }
}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */

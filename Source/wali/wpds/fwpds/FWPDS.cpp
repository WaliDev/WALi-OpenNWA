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
#include "wali/wpds/fwpds/InterGraph.hpp"

using namespace wali;
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

void
FWPDS::poststar( wfa::WFA& input, wfa::WFA& fa )
{
    poststarSetupFixpoint(input,fa);
    poststarComputeFixpoint(fa);
    currentOutputWFA=0;
}

struct FWPDSSourceFunctor : public wali::wfa::ConstTransFunctor
{
    InterGraph & gr;
    FWPDSSourceFunctor( InterGraph & p ) : gr(p) {}
    virtual void operator()( const wfa::Trans* t ) {
        gr.setSource(Transition(t->from(),t->stack(),t->to()),t->weight());
    }
};

struct FWPDSCompareFunctor : public wali::wfa::ConstTransFunctor
{
    InterGraph & gr;
    bool iseq;
    FWPDSCompareFunctor( InterGraph & p ) : gr(p) {
        iseq = true; 
    }

    virtual void operator()( const wfa::Trans* t ) {
    }

    std::ostream& print( std::ostream& out ) {
        if(!iseq) {
            out << "Not equal\n";
            assert(0);
        }
        return out;
    }
};

void
FWPDS::poststarComputeFixpoint( wfa::WFA& fa )
{
    LinkedTrans *t;
    if( get_from_worklist(t) ) {
        sem_elem_t se = t->weight();
        InterGraph* gr = new InterGraph(se,false,false);
        FWPDSSourceFunctor sources(*gr);
        fa.for_each(sources);
        {
            util::Timer timer("WPDS");

            do {
                post(t,fa, *gr);
            } while( get_from_worklist(t) );
        }
        {
            util::Timer timer("FWPDS");
            gr->setupInterSolution();
            gr->update_all_weights();
            // TODO
            //FWPDSCompareTransActionFunctor<T> comp(gr);
            //ca_out.for_each(comp);
            //comp.print(cout);
        }
    }
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
    //t->delta = dnew->zero();

    // For each forward rule of config
    // Apply rule to create new transition
    if( WALI_EPSILON != t->stack() )
    {
        Config::iterator fwit = config->begin();
        for( ; fwit != config->end() ; fwit++ ) {
            rule_t & r = *fwit;

            // Push and step rules use fromTrans
            Transition fromTrans(t->from(),t->stack(),t->to());

            // Step rule
            if( r->to_stack2() == WALI_EPSILON ) {
                update(r->to_state(), r->to_stack1(),t->to(),wghtOne,r->to() );
                gr.addEdge(fromTrans,
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

                // add source
                gr.setSource(Transition(r->to_state(),r->to_stack1(), gstate), wghtOne);
                // add edge (p,g,q) -> (p,g',(p,g'))
                gr.addCallEdge(fromTrans,Transition(r->to_state(),r->to_stack1(), gstate));
                // add edge (p,g,q) -> ((p,g'),rstk2,q)
                gr.addEdge(fromTrans,
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
                            gr.addEdge(Transition(tprime->from(), tprime->stack(), tprime->to()),
                                        Transition(teps->from(), teps->stack(), teps->to()),
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
        }
    }
}
/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */


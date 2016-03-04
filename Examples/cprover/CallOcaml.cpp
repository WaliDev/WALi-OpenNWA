#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "wali/wfa/Trans.hpp"
#include "NewtonOcamlInterface.hpp"
#include "Newton_Compare_Duet.hpp"

extern "C" {
#include <caml/mlvalues.h>
#include <caml/callback.h>
}
typedef wali::ref_ptr<graph::RegExp> reg_exp_t;
using namespace wali::wpds::fwpds;

static wali::Key st1()
{
    return getKey("Unique State Name");
}

static wali::Key stk(int k)
{
  return getKey(k);
}

std::vector<caml_rule> ruleHolder;
std::vector<caml_call_rule> callRuleHolder;
std::vector<caml_epsilon_rule> epsilonRuleHolder;
wali::Key entry_key;
wali::Key exit_key;
ref_ptr<DuetRel> compareWeight;

CAMLprim value compare_weights(Trans t) {
    CAMLparam0();
    CAMLlocal4(bval, nval, nCVal, sval);
    value * n_func = caml_named_value("normalize_callback");
    nCVal = caml_callback(*n_func, compareWeight->getValue());
    value * eq_func = caml_named_value("eq_callback");
    value * p_func = caml_named_value("print_callback");
    nval = caml_callback(*n_func, ((DuetRel*)(t.weight().get_ptr()))->getValue());
    sval = caml_callback(*p_func, nval);
    //std::cout << "WALI Weight: " << std::endl;
    //std::cout << String_val(sval) << std::endl;
    sval = caml_callback(*p_func, nCVal);
    //std::cout << "Duet Weight: " << std::endl;
    //std::cout << String_val(sval) << std::endl;
    bval = caml_callback2(*eq_func, nval, nCVal);


    if(Bool_val(bval))
    {
	std::cout << "Correct!" << std::endl;
    }
    else 
    {
	std::cout << "Failure!" << std::endl;
    }
    CAMLreturn(Val_unit);
}

void runFwpds(FWPDS * pds)
{
  #if ET_DBG == 1
    pds->print(std::cout);
#endif
    //reachPds->print(std::cout);
    WFA outfa;
    WFA outfa2;
    WFA fa;
    wali::Key acc = wali::getKeySpace()->getKey("accept");
    sem_elem_t x = pds->get_theZero();
    fa.addTrans(st1(),entry_key, acc, pds->get_theZero()->one());
    fa.setInitialState(st1());
    fa.addFinalState(acc);

#if ET_DBG == 1
	map<int, reg_exp_t> outNodeRegExpMap; // A map from a unique id of an outgoing node to the regular expression associated with it
	map<int, int> updateableMap;  //A map from an upadateable node number to the id of the node it depends on 
	map<int, int> oMap;
	map<int, pair< pair<int, int>, int> > mapBack;  //A map from the node index to the struct of the <<src,tgt>,stack> - used to insert weights back into wfa
	map<pair<pair<int, int>, int>, int> transMap;
	vector<int> differentiatedList; //A list of nodes with where the differential is to be taken
	map<std::pair<int, int>, std::pair<int, int> > mergeSrcMap; //The map that keeps track of the src of calls on call instructions
#endif
	
    //Calls the poststar functions
    cout << "[Newton Compare] Computing poststar..." << endl;
#if ET_DBG == 1
      pds->getOutRegExps(fa, outfa, outNodeRegExpMap, updateableMap, oMap, mapBack, transMap, differentiatedList, mergeSrcMap);
#endif
      pds->poststarIGR(fa,outfa);
    //static value * start_anal = NULL;
    //start_anal = caml_named_value("init_analysis");
    //caml_callback(*start_anal, Val_unit);

#if ET_DBG == 1
    cout << "Print Regular Expressions: " << endl;
    map<int, reg_exp_t>::iterator regExpIt = outNodeRegExpMap.begin();
    for (regExpIt; regExpIt != outNodeRegExpMap.end(); regExpIt++)
    {
 	regExpIt->second->print(std::cout);
	std::cout << std::endl << std::endl;
    }

#endif

    cout << "Finishing poststar: " << endl;
    outfa.path_summary_iterative_original(outfa.getSomeWeight()->one());

    fstream outfile("outfa.txt", fstream::out);
		  outfa.print(outfile);
		  outfile.close();

    Trans t;
    outfa.find(st1(), exit_key, acc, t);

    compare_weights(t);


    #undef flush
    std::cout << "Finished Printing" << std::endl << std::flush;
}

int main(int argc, char **argv)
{
    caml_startup(argv);
    FWPDS * pds = new FWPDS();

    ref_ptr<Reach> r = new Reach(true);
    for (std::vector<caml_rule>::iterator it = ruleHolder.begin(); it != ruleHolder.end(); it++)
    {
	pds->add_rule(st1(), it->first.first, st1(), it->first.second, it->second);
    }

    for (std::vector<caml_call_rule>::iterator itc = callRuleHolder.begin(); itc != callRuleHolder.end(); itc++)
    {
	pds->add_rule(st1(), itc->first.first, st1(), itc->first.second.first, itc->first.second.second, itc->second);
    }

    for (std::vector<caml_epsilon_rule>::iterator ite = epsilonRuleHolder.begin(); ite != epsilonRuleHolder.end(); ite++)
    {
	pds->add_rule(st1(), ite->first, st1(), ite->second);
    }

    WFA outfaNewton;
    goals::run_newton(outfaNewton, entry_key, pds, false);

    wali::Key acc = wali::getKeySpace()->getKey("accept");

    Trans t;
    outfaNewton.find(st1(), exit_key, acc, t);

    compare_weights(t);


    #undef flush
    std::cout << "Finished Printing" << std::endl << std::flush;

    return 0;
}

#include "NewtonOcamlInterface.hpp"

static wali::Key stk(int k)
{
  return getKey(k);
}


extern "C" {
  CAMLprim value add_wpds_rule(value weight, value v1, value v2) {
    CAMLparam3(weight, v1, v2);
    //CAMLlocal1(pass_fwpds);
    //FWPDS * fpds = (FWPDS*)pds;
    //fpds->print(std::cout);
    ref_ptr<DuetRel> w1 = DuetRel::MkDuetRel(weight);
    wali::Key k1 = stk(Int_val(v1));
    wali::Key k2 = stk(Int_val(v2));
    goals::ruleHolder.push_back(std::make_pair(std::make_pair(k1,k2),w1));
    //fpds->add_rule(st1(), k1, st1(), k2, w1);
    //pass_fwpds = (value) fpds;
    //CAMLreturn(pass_fwpds);
    CAMLreturn(Val_unit);
  }
}

extern "C" {
  CAMLprim value add_wpds_call_rule(value weight, value s1, value t1, value t2) {
    CAMLparam4(weight, s1, t1, t2);
    ref_ptr<DuetRel> w = DuetRel::MkDuetRel(weight);
    wali::Key ks = stk(Int_val(s1));
    wali::Key kt = stk(Int_val(t1));
    wali::Key kt2 = stk(Int_val(t2));
    goals::callRuleHolder.push_back(std::make_pair(std::make_pair(ks, std::make_pair(kt, kt2)), w));
    CAMLreturn(Val_unit);
  }
}

extern "C" {
  CAMLprim value add_wpds_epsilon_rule(value weight, value key) {
    CAMLparam2(weight, key);
    ref_ptr<DuetRel> w = DuetRel::MkDuetRel(weight);
    wali::Key k = stk(Int_val(key));
    goals::epsilonRuleHolder.push_back(std::make_pair(k,w));
    CAMLreturn(Val_unit);
  }
}
extern "C" {
  CAMLprim value set_vertices_wfa(value entry, value exit) {
    CAMLparam2(entry, exit);
    //FWPDS * fpds = (FWPDS*) pds;
    //WFA outfa;
    goals::entry_key = stk(Int_val(entry));
    goals::exit_key = stk(Int_val(exit));
    //doPostStar();
    //outfa.path_summary_tarjan_fwpds(true);
    //sem_elem_t wt = outfa.getState(outfa.getInitialState())->weight();
    CAMLreturn(Val_unit);
  }
}

extern "C" {
  CAMLprim value set_compare_weight(value cWeight){
    CAMLparam1(cWeight);
    //CAMLlocal1(sval);
    goals::compareWeight = DuetRel::MkDuetRel(cWeight);
    CAMLreturn(Val_unit);
  }
}


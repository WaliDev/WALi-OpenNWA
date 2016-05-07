#ifndef __NEWTON_OCAML_INTERFACE_HPP
#define __NEWTON_OCAML_INTERFACE_HPP 1

#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"
#include "wali/domains/duet/DuetRel.hpp"
#include "wali/domains/reach/Reach.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/KeySpace.hpp"
#include "wali/wfa/State.hpp"

extern "C" {
#include <stdio.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/alloc.h>
}

using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::ewpds;
using namespace wali::domains::duetrel;
using namespace wali::domains::reach;


namespace goals {

typedef std::pair<std::pair<wali::Key,wali::Key>, duetrel_t >  caml_rule;
typedef std::pair<std::pair<wali::Key,std::pair<wali::Key,wali::Key> >, duetrel_t > caml_call_rule;
typedef std::pair<wali::Key, duetrel_t > caml_epsilon_rule;
typedef std::pair<wali::Key, std::pair<duetrel_t,int> > caml_error_rule;
extern std::vector<caml_epsilon_rule> epsilonRuleHolder;
extern std::vector<caml_rule> ruleHolder;
extern std::vector<caml_call_rule> callRuleHolder;
extern std::vector<caml_error_rule> errorRuleHolder;
extern wali::Key entry_key;
extern wali::Key exit_key;
extern duetrel_t compareWeight;

}

#endif

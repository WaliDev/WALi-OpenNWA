#ifndef __NEWTON_OCAML_INTERFACE_HPP
#define __NEWTON_OCAML_INTERFACE_HPP 1

#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"
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

#include "NewtonCompare.hpp"

using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;
using namespace wali::wpds::fwpds;
using namespace wali::wpds::ewpds;
using namespace wali::domains::reach;

//namespace goals {

typedef std::pair<std::pair<wali::Key,wali::Key>, relation_t >  caml_rule;
typedef std::pair<std::pair<wali::Key,std::pair<wali::Key,wali::Key> >, relation_t > caml_call_rule;
typedef std::pair<wali::Key, relation_t > caml_epsilon_rule;
typedef std::pair<wali::Key, std::pair<relation_t,int> > caml_error_rule;
typedef std::pair<wali::Key, std::pair<int,int> > caml_print_hull_rule;

extern void push_call_rule(caml_call_rule);
extern void push_error_rule(caml_error_rule);
extern void push_epsilon_rule(caml_epsilon_rule);
extern void push_print_hull_rule(caml_print_hull_rule);
extern void push_rule(caml_rule);
extern void set_compare_weight(relation_t);
extern void set_vertices_wfa(wali::Key, wali::Key);
//}

#endif

#ifndef CPROVER_NEWTON_COMPARE_HPP
#define CPROVER_NEWTON_COMPARE_HPP
#ifdef USE_NWAOBDD 
#include "../turetsky/svn-repository/NWAOBDDRel.hpp"
using namespace wali::domains::nwaobddrel;
extern NWAOBDDContext * con;
#else
#include "wali/domains/binrel/BinRel.hpp"
using namespace wali::domains::binrel;
extern BddContext * con;
#endif
#endif

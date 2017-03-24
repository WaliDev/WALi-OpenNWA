#ifndef CPROVER_NEWTON_COMPARE_HPP
#define CPROVER_NEWTON_COMPARE_HPP

#ifdef USE_NWAOBDD 
#include "../turetsky/svn-repository/NWAOBDDRel.hpp"
using namespace wali::domains::nwaobddrel;
extern NWAOBDDContext * con;
typedef nwaobdd_t relation_t;
typedef NWAOBDDRel Relation;

#elif USE_BINREL
#include "wali/domains/binrel/BinRel.hpp"
using namespace wali::domains::binrel;
extern BddContext * con;
typedef binrel_t relation_t;
typedef BinRel Relation;

#elif USE_DUET
#include "wali/domains/duet/DuetRel.hpp"
using namespace wali::domains::duetrel;
typedef duetrel_t relation_t;
typedef DuetRel Relation;

#else 
#error Please define one of USE_NWAOBDD or USE_DUET or USE_BINREL

#endif

#endif

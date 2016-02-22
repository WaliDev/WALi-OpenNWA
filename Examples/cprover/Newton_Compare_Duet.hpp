#ifndef CPROVER_NEWTON_COMPARE_DUET_HPP
#define CPROVER_NEWTON_COMPARE_DUET_HPP 1
#include "wali/domains/duet/DuetRel.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/Key.hpp"

namespace goals
{
  double run_newton(wali::wfa::WFA& outfa, wali::Key entry_key, wali::wpds::fwpds::FWPDS * originalPds = NULL, bool canPrune = true);
}
#endif

/**
 * @author Akash Lal
 */

#include "Cfg.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/fwpds/SWPDS.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "Reach.hpp"
#include <string>
#include <sstream>
#include <fstream>


/* A class that illustrates how one can carry out program
 * analysis using the WALi package.
 */
class ProgAnalysis {

  // The Weighted Pushdown System. It is declared as a pointer so that
  // it can be initialized to any one of the several classes derived from
  // the WPDS base class. See TODO.
  wali::wpds::WPDS *pds;

  // We only create single state WPDSs. This is that unique state.
  wali::Key pds_state;

  // Entry node of the program
  wali::Key main_entry;

  // Store a weight. Because the weight domain is based on inheritance, it is useful to have
  // a weight lying around.
  wali::sem_elem_t se;

public:
  enum WpdsType { USING_WPDS, USING_EWPDS, USING_FWPDS, USING_SWPDS };
  enum SearchDirection { USE_FORWARD_SEARCH, USE_BACKWARD_SEARCH};

private:
  // Which wpds class to use for analysis?
  WpdsType wpdsType;

public:

  // Constructor that initializes the pds. It takes the following arguments:
  // The CFG of the main procedure of the program.
  // The type of WPDS class to use for analysis
  ProgAnalysis(CFG *main, WpdsType t);

  // Calculate MOP(main->entry, n)
  wali::sem_elem_t computeMOP(CFGNode *n, SearchDirection d);

  // Calculate MOP(main->entry, stack)
  wali::sem_elem_t computeMOP(const std::list<CFGNode *> &st, SearchDirection d);

private:

  void doForwardSearch(wali::wfa::WFA &answer);
  void doBackwardSearch(wali::Key node, wali::wfa::WFA &answer);
  void doBackwardSearch(std::vector<wali::Key> &node_stack, wali::wfa::WFA &answer);
  void traverseCFG(CFG *cfg, std::set<CFG *> &cfgs, std::set<CFG *> &cfgs_done);
};





/**
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include <string>
#include <list>
#include <set>
#include <vector>

class CFG;
class CFGNode;
namespace wali {
  namespace wpds {
    class WPDS;
  }
  namespace wfa {
    class WFA;
  }
}

/* A class that illustrates how one can carry out program
 * analysis using the WALi package.
 */
class ProgAnalysis {

public:
  enum WpdsType { USING_WPDS, USING_EWPDS, USING_FWPDS, USING_SWPDS };
  enum SearchDirection { USE_FORWARD_SEARCH, USE_BACKWARD_SEARCH};

  // Constructor that initializes the pds. It takes the following arguments:
  // The CFG of the main procedure of the program.
  // The type of WPDS class to use for analysis
  ProgAnalysis(CFG *main, WpdsType t, bool witnesses=false);

  // @return [true] if [this] was created with [witnesses] set to true.
  bool isWitnessed() const;

  // Calculate MOP(main->entry, n)
  wali::sem_elem_t computeMOP(CFGNode *n, SearchDirection d);

  // Calculate MOP(main->entry, stack)
  wali::sem_elem_t computeMOP(const std::list<CFGNode *> &st, SearchDirection d);

private:
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

  // Which wpds class to use for analysis?
  WpdsType wpdsType;

  // If true, then the *WPDSs will compute witnesses for a query.
  // @see wali::witness::Witness
  const bool b_witnesses;

private:

  void doForwardSearch(wali::wfa::WFA &answer);
  void doBackwardSearch(wali::Key node, wali::wfa::WFA &answer);
  void doBackwardSearch(std::vector<wali::Key> &node_stack, wali::wfa::WFA &answer);
  void traverseCFG(CFG *cfg, std::set<CFG *> &cfgs, std::set<CFG *> &cfgs_done);
};





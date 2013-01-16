// ::wali::domains::binrel
#include "BinRel.hpp"
// ::std
#include <algorithm>

#include "buddy/kernel.h"
#include "buddy/bdd.h"
#include "nwa_detensor.hpp"

#ifdef NWA_DETENSOR

using namespace std;
using namespace wali;
using namespace wali::domains::binrel;
using namespace wali::domains::binrel::details;
using namespace opennwa;

// //////////////////////////////////////////
// These functions provide a sneak-peek into the buddy bit-level
// variable ordering.
// Setup maps to enable quick lookup of information
void BddContext::setupLevelArray()
{
  vocLevels.clear();
  for(BddContext::const_iterator cit = this->begin(); cit != this->end(); ++cit){
    int const * vars;
    bddinfo_t const varInfo = cit->second;
    // XXX:HACK BddContext is not supposed to know that __regA etc exist.
    // Try: skip this check and see if everything works out OK. Try the check below instead.
    if(cit->first.compare("__regSize") == 0 || cit->first.compare("__regA") == 0 || cit->first.compare("__regB") == 0)
      continue;
    //Try this instead:
    //if(varInfo->maxVal > 2) continue;

    // Only supports boolean variables.
    assert(varInfo->maxVal == 2);

    vars = fdd_vars(varInfo->tensor1Lhs);
    assert(vars != NULL);
    vocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->tensor2Lhs);
    assert(vars != NULL);
    vocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->tensor1Rhs);
    assert(vars != NULL);
    vocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->tensor2Rhs);
    assert(vars != NULL);
    vocLevels.push_back(bdd_var2level(vars[0]));
  }
  sort(vocLevels.begin(), vocLevels.end());
}

BddContext::VocLevelArray const& BddContext::getLevelArray()
{
  return vocLevels;
}

unsigned BddContext::numVars()
{
  return this->size();
}

// ///////////////////////////////////////////
// Modified dot output from a buddy bdd.
// The output nodes are labeled with the actual levels rather than with
// internal variable nubmers (as in the buddy provided dot output 
extern "C"
{
  static void bdd_fprintdot_levels_rec(FILE* ofile, BDD r)
  {
    if (ISCONST(r) || MARKED(r))
      return;

    fprintf(ofile, "%d [label=\"", r);
    fprintf(ofile, "%d",LEVEL(r));
    fprintf(ofile, "\"];\n");

    fprintf(ofile, "%d -> %d [style=dotted];\n", r, LOW(r));
    fprintf(ofile, "%d -> %d [style=filled];\n", r, HIGH(r));

    SETMARK(r);

    bdd_fprintdot_levels_rec(ofile, LOW(r));
    bdd_fprintdot_levels_rec(ofile, HIGH(r));
  }
}

void bdd_printdot_levels(bdd r)
{
  bdd_fprintdot_levels(stdout, r);
}

int bdd_fnprintdot_levels(char *fname, bdd r)
{
  FILE *ofile = fopen(fname, "w");
  if (ofile == NULL)
    return bdd_error(BDD_FILE);
  bdd_fprintdot_levels(ofile, r);
  fclose(ofile);
  return 0;
}

void bdd_fprintdot_levels(FILE* ofile, bdd r)
{
  fprintf(ofile, "digraph G {\n");
  fprintf(ofile, "0 [shape=box, label=\"0\", style=filled, shape=box, height=0.3, width=0.3];\n");
  fprintf(ofile, "1 [shape=box, label=\"1\", style=filled, shape=box, height=0.3, width=0.3];\n");

  bdd_fprintdot_levels_rec(ofile, r.root);

  fprintf(ofile, "}\n");

  bdd_unmark(r.root);
}


// //////////////////////////////////////////
// The algorithm!!!

size_t hash_value(bdd const& b)
{
  return b.root;
}
bdd BinRel::detensorViaNwa()
{
  Nwa nwa;
  populateNwa(nwa);
  return bddfalse;
}

void BinRel::populateNwa(Nwa& nwa)
{
  BinRel::StateTranslationTable tTable;
  tabulateStates(nwa, tTable, con->vocLevels[0], rel);
}

void BinRel::tabulateStates(Nwa& nwa, BinRel::StateTranslationTable& tTable, unsigned v, bdd r)
{
  tTable.find(StateTranslationKey(v,r));
  nwa.clear();
}
#endif //#ifdef NWA_DETENSOR

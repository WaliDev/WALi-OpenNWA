// ::wali::domains::binrel
#include "BinRel.hpp"
// ::std
#include <algorithm>
#include <fstream>
#include <sstream>


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
  tensorVocLevels.clear();
  baseLhsVocLevels.clear();
  baseRhsVocLevels.clear();
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
    tensorVocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->tensor2Lhs);
    assert(vars != NULL);
    tensorVocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->tensor1Rhs);
    assert(vars != NULL);
    tensorVocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->tensor2Rhs);
    assert(vars != NULL);
    tensorVocLevels.push_back(bdd_var2level(vars[0]));

    vars = fdd_vars(varInfo->baseLhs);
    assert(vars != NULL);
    baseLhsVocLevels.push_back(bdd_var2level(vars[0]));
    vars = fdd_vars(varInfo->baseRhs);
    assert(vars != NULL);
    baseRhsVocLevels.push_back(bdd_var2level(vars[0]));
  }
  sort(tensorVocLevels.begin(), tensorVocLevels.end());
}

BddContext::VocLevelArray const& BddContext::getLevelArray()
{
  return tensorVocLevels;
}

unsigned BddContext::numVars() const
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
    fprintf(ofile, "%d:%d",r,LEVEL(r));
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

// /////////////////////////////////////////
// Class DetensorWeightGen

sem_elem_t DetensorWeightGen::getOne() const
{
  return NULL;
}

sem_elem_t DetensorWeightGen::getWeight(Key, ClientInfoRefPtr, Key, Kind, Key, ClientInfoRefPtr) const
{
  return NULL;
}

void DetensorWeightGen::setWeight(Kind,Key,Key,Key,binrel_t)
{
}

void DetensorWeightGen::clear()
{
}
// //////////////////////////////////////////
// The algorithm!!!
static inline bool post_upperplies(unsigned const v)
{
  return v % 2 == 0;
}
static inline bool post_lowerplies(unsigned const v)
{
  return !post_upperplies(v);
}

size_t hash_value(bdd const& b)
{
  return b.root;
}

bdd BinRel::detensorViaNwa()
{
  Nwa nwa;
  DetensorWeightGen wts; 
  populateNwa(nwa, wts);
  return bddfalse;
}

void BinRel::populateNwa(Nwa& nwa, DetensorWeightGen& wts)
{
  nwa.clear();
  //wts.clear();
  acceptState = getKey("__accept_state_detensor_nwa");
  nwa.addFinalState(acceptState);
  // I can safely assume that the hash for all bdd's will be non-negative.  It
  // is risky to use the hash of bddfalse for reject state because bddfalse
  // might be NULL
  rejectState = getKey(-1);
  low = getKey("__trans_for_bdd_low_edge_detensor_nwa");
  high = getKey("__trans_for_bdd_high_edge_detensor_nwa");
  tTable.clear();
  for(LevelToStatesTable::iterator it = lTable.begin(); it != lTable.end(); ++it)
    it->clear();
  lTable.clear();
  for(unsigned i=0; i<con->numVars(); ++i)
    lTable.push_back(vector< State >());
  tabulateStates(nwa, 0, rel);
  cout << "#Nwa State created: " << tTable.size() << endl;
  visited.clear();
  generateTransitions(nwa, wts, 0, rel); 
  //dbg
  filebuf fb;
  fb.open("detensor_nwa.dot", ios::out);
  ostream o(&fb);
  nwa.print_dot(o, string("Whatever"), false);
  fb.close();
}

void BinRel::tabulateStates(Nwa& nwa, unsigned v, bdd n)
{
  // We don't want any state for the false terminal of bdd
  if(n == bddfalse)
    return;
  StateTranslationKey k = StateTranslationKey(v,n);
  // Already visited, skip. Bdd is a dag, only walk each node once.
  if(tTable.find(k) != tTable.end())
    return;
  // Last level, must be bddtrue, since bddfalse has already been taken care of
  if(v == 4 * con->numVars()){
    assert(n == bddtrue);
    StateSet fStates = nwa.getFinalStates();
    assert(fStates.size() == 1);
    tTable[k] = *(fStates.begin());
    return;
  }
  // Fast but cryptic
  //boost::hash<StateTranslationKey> keyHash;
  //State q = getKey(keyHash(k));
  // Slow but informative
  stringstream ss;
  ss << hash_value(n) << "::" << v;
  State q = getKey(ss.str());

  if(!nwa.addState(q))
    assert("Hash collision? or flawed logic?");
  tTable[k] = q;
  if(v < 2 * con->numVars() && !post_upperplies(v))
    lTable[(v-1)/2].push_back(q);
  if(v == 0)
    nwa.addInitialState(q);
  if(con->tensorVocLevels[v] < bdd_var2level(bdd_var(n))){
    // bdd skipped a level, create extra nwa states
    tabulateStates(nwa, v+1, n);
  }else{
    // tabulate States for children
    tabulateStates(nwa, v+1, bdd_low(n));
    tabulateStates(nwa, v+1, bdd_high(n));
  }
}

State BinRel::generateTransitions(Nwa& nwa, DetensorWeightGen& wts, unsigned v, bdd n)
{
  if(n == bddfalse)
    return rejectState;
  if(v == 2 * con->numVars())
    //process the lower plies of bdd
    return generateTransitionsLowerPlies(nwa, wts, v, n);
  // Process the upper plies
  StateTranslationKey k = StateTranslationKey(v,n);
  State q = tTable[k];
  if(visited.find(q) != visited.end())
    // q already processed
    return q;
  visited.insert(q);

  if(con->tensorVocLevels[v] < bdd_var2level(bdd_var(n))){
    // visit virtual child(v+1, n)
    State qprime = generateTransitions(nwa, wts, v+1, n);
    if(qprime != rejectState){
      if(post_upperplies(v)){
        //Create internal transitions
        nwa.addInternalTrans(q, low, qprime);
        wts.setWeight(WeightGen::INTRA, q, low, qprime, new BinRel(con, bddtrue)); 
        nwa.addInternalTrans(q, high, qprime);
        wts.setWeight(WeightGen::INTRA, q, high, qprime, new BinRel(con, bddtrue)); 
      }else{
        //Create call transitions
        nwa.addCallTrans(q, low, qprime);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, low, qprime, new BinRel(con, bdd_nithvar(bdd_level2var(con->baseLhsVocLevels[v]))));
        nwa.addCallTrans(q, high, qprime);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, high, qprime, new BinRel(con, bdd_ithvar(bdd_level2var(con->baseLhsVocLevels[v]))));
      }
    }
  }else{
    // visit actual children
    State qlow = generateTransitions(nwa, wts, v+1, bdd_low(n));
    State qhigh = generateTransitions(nwa, wts, v+1, bdd_high(n));
    if(qlow != rejectState){
      if(post_upperplies(v)){
        nwa.addInternalTrans(q, low, qlow);
        wts.setWeight(WeightGen::INTRA, q, low, qlow, new BinRel(con, bddtrue)); 
      }else{
        nwa.addCallTrans(q, low, qlow);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, low, qlow, new BinRel(con, bdd_nithvar(bdd_level2var(con->baseLhsVocLevels[v]))));
      }
    }
    if(qhigh != rejectState){
      if(post_upperplies(v)){
        nwa.addInternalTrans(q, high, qhigh);
        wts.setWeight(WeightGen::INTRA, q, high, qhigh, new BinRel(con, bddtrue));
      }else{
        nwa.addCallTrans(q, high, qhigh);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, high, qhigh, new BinRel(con, bdd_ithvar(bdd_level2var(con->baseLhsVocLevels[v]))));
      }
    }
  }
  return q;
}

State BinRel::generateTransitionsLowerPlies(Nwa& nwa, DetensorWeightGen& wts, unsigned v, bdd n)
{
  if(n == bddfalse)
    return rejectState;
  if(v == 4 * con->numVars())
    return acceptState;
  StateTranslationKey k = StateTranslationKey(v,n);
  State q = tTable[k];
  if(visited.find(q) != visited.end())
    //q already processed
    return q;
  visited.insert(q);
  if(con->tensorVocLevels[v] < bdd_var2level(bdd_var(n))){
    // visit virtual child n+1
    State qprime = generateTransitionsLowerPlies(nwa, wts, v+1, n);
    if(qprime != rejectState){
      if(post_lowerplies(v)){
        nwa.addInternalTrans(q, low, qprime);
        wts.setWeight(WeightGen::INTRA, q, low, qprime, new BinRel(con, bdd_nithvar(bdd_level2var(con->baseRhsVocLevels[v]))));
        nwa.addInternalTrans(q, high, qprime);
        wts.setWeight(WeightGen::INTRA, q, high, qprime, new BinRel(con, bdd_ithvar(bdd_level2var(con->baseRhsVocLevels[v]))));
      }else{
        vector< State > const& callStates = lTable[(4 * con->numVars() - v - 1)/2];
        for(vector< State >::const_iterator cit = callStates.begin(); cit != callStates.end(); ++cit){
          nwa.addReturnTrans(q, *cit, low, qprime);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, low, qprime, new BinRel(con, bddtrue));
          nwa.addReturnTrans(q, *cit, high, qprime);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, high, qprime, new BinRel(con, bddtrue));
        }
      }
    }
  }else{
    // visit actual children
    State qlow = generateTransitionsLowerPlies(nwa, wts, v+1, bdd_low(n));
    State qhigh = generateTransitionsLowerPlies(nwa, wts, v+1, bdd_high(n));
    if(qlow != rejectState){
      if(post_lowerplies(v)){
        nwa.addInternalTrans(q, low, qlow);
        wts.setWeight(WeightGen::INTRA, q, low, qlow, new BinRel(con, bdd_nithvar(bdd_level2var(con->baseRhsVocLevels[v]))));
      }else{
        vector< State > const& callStates = lTable[(4 * con->numVars() - v - 1)/2];
        for(vector< State >::const_iterator cit = callStates.begin(); cit != callStates.end(); ++cit){
          nwa.addReturnTrans(q, *cit, low, qlow);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, low, qlow, new BinRel(con, bddtrue));
        }
      }
    }
    if(qhigh != rejectState){
      if(post_lowerplies(v)){
        nwa.addInternalTrans(q, high, qhigh);
        wts.setWeight(WeightGen::INTRA, q, high, qhigh, new BinRel(con, bdd_ithvar(bdd_level2var(con->baseRhsVocLevels[v]))));
      }else{
        vector< State > const& callStates = lTable[(4 * con->numVars() - v - 1)/2];
        for(vector< State >::const_iterator cit = callStates.begin(); cit != callStates.end(); ++cit){
          nwa.addReturnTrans(q, *cit, high, qhigh);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, high, qhigh, new BinRel(con, bddtrue));
        }
      }
    }
  }
  return q;
}
#endif //#ifdef NWA_DETENSOR

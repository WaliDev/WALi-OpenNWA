// ::wali::domains::binrel
#include "BinRel.hpp"
// ::std
#include <algorithm>
#include <fstream>
#include <sstream>


#include "buddy/kernel.h"
#include "buddy/bdd.h"
#include "nwa_detensor.hpp"
// ::wali
#include "wali/KeyPairSource.hpp"
// ::opennwa::nwa_pds
#include "opennwa/nwa_pds/conversions.hpp"
// ::opennwa::construct
#include "opennwa/construct/intersect.hpp"
// ::wali::wfa
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"

#if (NWA_DETENSOR == 1)

using namespace std;
using namespace std::tr1;
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
  unsigned numVarsToSkip = 0;
  tensorVocLevels.clear();
  baseLhsVocLevels.clear();
  baseRhsVocLevels.clear();
  for(BddContext::const_iterator cit = this->begin(); cit != this->end(); ++cit){
    int const * vars;
    bddinfo_t const varInfo = cit->second;
    // XXX:HACK BddContext is not supposed to know that __regA etc exist.
    // Try: skip this check and see if everything works out OK. Try the check below instead.
    // This is a problem throught this function.
    if(cit->first.compare("__regSize") == 0 || cit->first.compare("__regA") == 0 || cit->first.compare("__regB") == 0){
      ++numVarsToSkip;
      continue;
    }
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

  // This raw bddpair is used to move stuff around for detensor based tensor
  unsigned numUsefulVars = this->size() - numVarsToSkip;
  int * base = new int[2 * numUsefulVars];
  int * tensor2 = new int[2 * numUsefulVars];
  int vari = 0;
  for(std::map<const std::string, bddinfo_t>::const_iterator ci = this->begin(); ci != this->end(); ++ci){
    if(ci->first.compare("__regSize") == 0 || ci->first.compare("__regA") == 0 || ci->first.compare("__regB") == 0)
      continue;
    bddinfo_t varInfo = ci->second;
    base[vari] = varInfo->baseLhs;
    base[vari+1] = varInfo->baseRhs;
    vari+=2;
  }
  vari = 0;
  for(std::map<const std::string, bddinfo_t>::const_reverse_iterator cri = this->rbegin(); cri != this->rend(); ++cri){
    if(cri->first.compare("__regSize") == 0 || cri->first.compare("__regA") == 0 || cri->first.compare("__regB") == 0)
      continue;
    bddinfo_t varInfo = cri->second;
    tensor2[vari] = varInfo->tensor2Lhs;
    tensor2[vari+1] = varInfo->tensor2Rhs;
    vari+=2;
  }
  fdd_setpairs(rawMove2Tensor2.get(), base, tensor2, 2 * numUsefulVars);
  delete [] base;
  delete [] tensor2;

  t1OneBdd = bddtrue;
  for(BddContextIter varIter = this->begin(); varIter != this->end();  ++varIter){
    if(varIter->first.compare("__regSize") == 0 || varIter->first.compare("__regA") == 0 || varIter->first.compare("__regB") == 0)
      continue;
    bddinfo_t varInfo = varIter->second;
    t1OneBdd = t1OneBdd & fdd_equals(varInfo->tensor1Lhs,  varInfo->tensor1Rhs);
  }
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

int bdd_fnprintdot_levels(char const * fname, bdd r)
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
// Class IntersectedNwa
bool DetensorNwa::stateIntersect(Nwa const& first, State state1, Nwa const& second, State state2, State& resSt, ClientInfoRefPtr& resCI)
{
  if(Nwa::stateIntersect(first, state1, second, state2, resSt, resCI)){
    resSt = getKey(state1, state2);
    return true;
  }
  return false;
}

// /////////////////////////////////////////
// Class DetensorWeight
detensor_weight_t DetensorWeight::detensor_weight_one = NULL;
detensor_weight_t DetensorWeight::detensor_weight_zero = NULL;
// /////////////////////////////////////////
// Class DetensorWeightGen

sem_elem_t DetensorWeightGen::getOne() const
{
  return theOne;
}

sem_elem_t DetensorWeightGen::getWeight(Key src, ClientInfoRefPtr, Key sym, Kind kind , Key tgt, ClientInfoRefPtr) const
{
  KeyPairSource * kps;
  kps = dynamic_cast<KeyPairSource*>(getKeySource(src).get_ptr());
  src = kps->first();
  kps = dynamic_cast<KeyPairSource*>(getKeySource(tgt).get_ptr());
  tgt = kps->first();
  TransQuad q = make_tuple(kind, src, sym, tgt);
  TransWeightMap::const_iterator rit = twmap.find(q);
  if(rit == twmap.end())
    assert(0);
  detensor_weight_t ans = rit->second;
  return ans.get_ptr();
}

bool DetensorWeightGen::setWeight(Kind kind, Key src, Key sym, Key tgt, detensor_weight_t b)
{
  TransQuad q = make_tuple(kind, src, sym, tgt);
  if(twmap.find(q) != twmap.end()){
    detensor_weight_t old = twmap[q];
    twmap[q] = dynamic_cast<DetensorWeight*>(old->combine(b.get_ptr()).get_ptr());
    return false;
  }else{
    twmap[q] = b;
    return true;
  }
}

void DetensorWeightGen::clear()
{
  twmap.clear();
}

size_t DetensorWeightGen::TransQuadHash::operator () (DetensorWeightGen::TransQuad const& q) const
{
  DetensorWeightGen::Kind kind;
  Key src, sym, tgt;
  tie(kind, src, sym, tgt) = q;
  return (kind << 30) + src + (sym << 8) + (tgt << 16);
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
  if(rel == bddfalse)
    return bddfalse;
  if(rel == bddtrue)
    return bddtrue;

  DetensorNwa inwa;
  DetensorNwa cnwa;
  DetensorNwa nwa;
  DetensorWeight::initialize();
  DetensorWeightGen wts(DetensorWeight::detensor_weight_one); 
  populateNwa(inwa, wts);
  setupConstraintNwa(cnwa);
  opennwa::construct::intersect(nwa, inwa, cnwa);

  // Create the query automaton
  wali::wfa::WFA infa;
  initialStateWfa = opennwa::nwa_pds::getProgramControlLocation();
  acceptStateWfa = getKey("__accept_state_detensor_wfa");
  const StateSet& nwaInitStates = nwa.getInitialStates();
  for(StateSet::const_iterator cit = nwaInitStates.begin(); cit != nwaInitStates.end(); ++cit)
    infa.addTrans(initialStateWfa, *cit, acceptStateWfa, DetensorWeight::detensor_weight_one);
  infa.setInitialState(initialStateWfa);
  infa.addFinalState(acceptStateWfa);
  wali::wfa::WFA outfa = nwa.poststar(infa, wts);
  detensor_weight_t ans = DetensorWeight::detensor_weight_zero;
  const StateSet& nwaFinalStates = nwa.getFinalStates();
  for(StateSet::const_iterator cit = nwaFinalStates.begin(); cit != nwaFinalStates.end(); ++cit){
    wali::wfa::Trans ansTrans;  
    outfa.find(initialStateWfa, *cit, acceptStateWfa, ansTrans);
    ans = static_cast<DetensorWeight*>(ans->combine(static_cast<DetensorWeight*>(ansTrans.weight().get_ptr())).get_ptr());
  }

#if 0
  //dbg
  char const * oldf = "original_obdd.dot";
  bdd_fnprintdot_levels(oldf, rel);
  {
    filebuf fb;
    fb.open("obdd2nwa.dot", ios::out);
    ostream o(&fb);
    inwa.print_dot(o, string("Whatever"), false);
    fb.close();
  }
  {
    filebuf fb;
    fb.open("detensor_nwa.dot", ios::out);
    ostream o(&fb);
    nwa.print_dot(o, string("Whatever"), false);
    fb.close();
  }
  {
    filebuf fb;
    fb.open("constraint_nwa.dot", ios::out);
    ostream o(&fb);
    cnwa.print_dot(o, string("Whatever"), false);
    fb.close();
  }
  {
    filebuf fb;
    fb.open("infa.dot", ios::out);
    ostream o(&fb);
    infa.print_dot(o, true); 
    fb.close();
  }
  {
    filebuf fb;
    fb.open("outfa.dot", ios::out);
    ostream o(&fb);
    outfa.print_dot(o, true); 
    fb.close();
  }

  if(ans == NULL)
    cout << "Answer: NULL\n";
  else{
    cout << "Answer: " << fddset << ans->getBdd() << "\n";
    char const * newf = "final_obdd.dot";
    bdd_fnprintdot_levels(newf, ans->getBdd());
  }
#endif
  return (ans == NULL) ? bddfalse : ans->getBdd();
}

void BinRel::populateNwa(DetensorNwa& nwa, DetensorWeightGen& wts)
{
  nwa.clear();
  //wts.clear();
  acceptStateNwa = getKey("__accept_state_detensor_nwa");
  nwa.addFinalState(acceptStateNwa);
  // I can safely assume that the hash for all bdd's will be non-negative.  It
  // is risky to use the hash of bddfalse for reject state because bddfalse
  // might be NULL
  rejectState = getKey(-1);
  low = getKey("t0");
  high = getKey("t1");
  tTable.clear();
  for(LevelToStatesTable::iterator it = lTable.begin(); it != lTable.end(); ++it)
    it->clear();
  lTable.clear();
  for(unsigned i=0; i<con->numVars(); ++i)
    lTable.push_back(vector< State >());

  // Transpose tensor1 levels.
  bdd relt = bdd_replace(rel, con->tensor1Swap.get()); 
  
  tabulateStates(nwa, 0, relt);
  //cout << "#Nwa State created: " << tTable.size() << endl;
  visited.clear();
  generateTransitions(nwa, wts, 0, relt); 
}

void BinRel::setupConstraintNwa(DetensorNwa& nwa)
{
  // Fast by cryptic
  // boost::hash<unsigned> unsignedHash;
  // size_t magik = 354958734;
  nwa.clear();
  if(con->tensorVocLevels.size() == 0)
    return;
  std::tr1::unordered_map < unsigned, std::pair< State, State > > callStatesMap;
  State upperPrimedState;
  {
    //upperPrimed = magik + unsignedHash(con->tensorVocLevels[0]);
    stringstream ss;
    ss << "u'::" << 0 << "::" << con->tensorVocLevels[0];
    upperPrimedState = getKey(ss.str());
  }
  nwa.addInitialState(upperPrimedState);

  for(unsigned level = 0; level < con->tensorVocLevels.size()/2; level += 2){
    // Split on symbol
    State highState; 
    {
      //highState = ((magik + unsignedHash(con->tensorVocLevels[level])) << 1) + 1 ;
      stringstream ss;
      ss << "u1::" << level+1 << "::" << con->tensorVocLevels[level+1];
      highState = getKey(ss.str());
    }
    nwa.addState(highState);
    State lowState;
    {
      //lowState = (magik + unsignedHash(con->tensorVocLevels[level])) << 1;
      stringstream ss;
      ss << "u0::" << level+1 << "::" << con->tensorVocLevels[level+1];
      lowState = getKey(ss.str());
    }
    nwa.addState(lowState);
    nwa.addInternalTrans(upperPrimedState, high, highState);
    nwa.addInternalTrans(upperPrimedState, low, lowState);
    callStatesMap[level/2] = std::pair< State, State >(highState, lowState);

    //Merge with call transition
    {
      //upperPrimed = magik + unsignedHash(con->tensorVocLevels[level+2]);
      stringstream ss;
      if(level+2 < con->tensorVocLevels.size()/2)
        ss << "u'::" << level+2 << "::" << con->tensorVocLevels[level+2];
      else
        ss << "l::" << level+2 << "::" << con->tensorVocLevels[level+2];
      upperPrimedState = getKey(ss.str());
    }
    nwa.addCallTrans(highState, high, upperPrimedState);
    nwa.addCallTrans(highState, low, upperPrimedState);
    nwa.addCallTrans(lowState, low, upperPrimedState);
    nwa.addCallTrans(lowState, high, upperPrimedState);
  }

  State lowerPlainState = upperPrimedState;
  for(unsigned level = con->tensorVocLevels.size() / 2; level < con->tensorVocLevels.size(); level += 2){
    // Return transitions
    std::pair< State, State > callStates = callStatesMap[(con->numVars()*4 - level)/2 - 1];
    State lowerPrimedState;
    {
      //lowerPrimedState = magik + unsignedHash(con->tensorVocLevels[level+1]);
      stringstream ss;
      ss << "l'::" << level+1 << "::" << con->tensorVocLevels[level+1];
      lowerPrimedState = getKey(ss.str());
    }
    nwa.addReturnTrans(lowerPlainState, callStates.first, high, lowerPrimedState);
    nwa.addReturnTrans(lowerPlainState, callStates.second, low, lowerPrimedState);

    // Internal transition
    if(level+2 < con->tensorVocLevels.size()){
      // lowerPlainState = magik + unsignedHash(con->tensorVocLevels[level+2]);
      stringstream ss;
      ss << "l'::" << level+2 << "::" << con->tensorVocLevels[level+2];
      lowerPlainState = getKey(ss.str());
    }else{
      lowerPlainState = getKey("intersectNwa::finalState");
    }
    nwa.addInternalTrans(lowerPrimedState, high, lowerPlainState);
    nwa.addInternalTrans(lowerPrimedState, low, lowerPlainState);
  }
  nwa.addFinalState(lowerPlainState);
}

void BinRel::tabulateStates(DetensorNwa& nwa, unsigned v, bdd n)
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
  if(v == 0)
    ss << "::NwaInitialState";
  State q = getKey(ss.str());

  if(!nwa.addState(q))
    assert("Hash collision? or flawed logic?");
  tTable[k] = q;
  if(v < 2 * con->numVars() && !post_upperplies(v))
    lTable[(v-1)/2].push_back(q);
  if(v == 0){
    nwa.addInitialState(q);
    initialStateNwa = q;
  }
  if((n == bddtrue && v < 4 * con->numVars()) || (con->tensorVocLevels[v] < bdd_var2level(bdd_var(n)))){
    // bdd skipped a level, create extra nwa states
    tabulateStates(nwa, v+1, n);
  }else{
    // tabulate States for children
    tabulateStates(nwa, v+1, bdd_low(n));
    tabulateStates(nwa, v+1, bdd_high(n));
  }
}

State BinRel::generateTransitions(DetensorNwa& nwa, DetensorWeightGen& wts, unsigned v, bdd n)
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

  if((n == bddtrue && v < 4 * con->numVars()) || (con->tensorVocLevels[v] < bdd_var2level(bdd_var(n)))){
    // visit virtual child(v+1, n)
    State qprime = generateTransitions(nwa, wts, v+1, n);
    if(qprime != rejectState){
      if(post_upperplies(v)){
        //Create internal transitions
        nwa.addInternalTrans(q, low, qprime);
        wts.setWeight(WeightGen::INTRA, q, low, qprime, DetensorWeight::detensor_weight_one); 
        nwa.addInternalTrans(q, high, qprime);
        wts.setWeight(WeightGen::INTRA, q, high, qprime, DetensorWeight::detensor_weight_one); 
      }else{
        //Create call transitions
        nwa.addCallTrans(q, low, qprime);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, low, qprime, new DetensorWeight(bdd_nithvar(bdd_level2var(con->baseLhsVocLevels[(v-1)/2]))));
        nwa.addCallTrans(q, high, qprime);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, high, qprime, new DetensorWeight(bdd_ithvar(bdd_level2var(con->baseLhsVocLevels[(v-1)/2]))));
      }
    }
  }else{
    // visit actual children
    State qlow = generateTransitions(nwa, wts, v+1, bdd_low(n));
    State qhigh = generateTransitions(nwa, wts, v+1, bdd_high(n));
    if(qlow != rejectState){
      if(post_upperplies(v)){
        nwa.addInternalTrans(q, low, qlow);
        wts.setWeight(WeightGen::INTRA, q, low, qlow, DetensorWeight::detensor_weight_one); 
      }else{
        nwa.addCallTrans(q, low, qlow);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, low, qlow, new DetensorWeight(bdd_nithvar(bdd_level2var(con->baseLhsVocLevels[(v-1)/2]))));
      }
    }
    if(qhigh != rejectState){
      if(post_upperplies(v)){
        nwa.addInternalTrans(q, high, qhigh);
        wts.setWeight(WeightGen::INTRA, q, high, qhigh, DetensorWeight::detensor_weight_one);
      }else{
        nwa.addCallTrans(q, high, qhigh);
        wts.setWeight(WeightGen::CALL_TO_ENTRY, q, high, qhigh, new DetensorWeight(bdd_ithvar(bdd_level2var(con->baseLhsVocLevels[(v-1)/2]))));
      }
    }
  }
  return q;
}

State BinRel::generateTransitionsLowerPlies(DetensorNwa& nwa, DetensorWeightGen& wts, unsigned v, bdd n)
{
  if(n == bddfalse)
    return rejectState;
  if(v == 4 * con->numVars())
    return acceptStateNwa;
  StateTranslationKey k = StateTranslationKey(v,n);
  State q = tTable[k];
  if(visited.find(q) != visited.end())
    //q already processed
    return q;
  visited.insert(q);
  if((n == bddtrue && v < 4 * con->numVars()) || (con->tensorVocLevels[v] < bdd_var2level(bdd_var(n)))){
    // visit virtual child n+1
    State qprime = generateTransitionsLowerPlies(nwa, wts, v+1, n);
    if(qprime != rejectState){
      if(post_lowerplies(v)){
        nwa.addInternalTrans(q, low, qprime);
        wts.setWeight(WeightGen::INTRA, q, low, qprime, new DetensorWeight(bdd_nithvar(bdd_level2var(con->baseRhsVocLevels[(4*con->numVars() - v - 1)/2]))));
        nwa.addInternalTrans(q, high, qprime);
        wts.setWeight(WeightGen::INTRA, q, high, qprime, new DetensorWeight(bdd_ithvar(bdd_level2var(con->baseRhsVocLevels[(4*con->numVars() - v - 1)/2]))));
      }else{
        vector< State > const& callStates = lTable[(4 * con->numVars() - v - 1)/2];
        for(vector< State >::const_iterator cit = callStates.begin(); cit != callStates.end(); ++cit){
          nwa.addReturnTrans(q, *cit, low, qprime);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, low, qprime, DetensorWeight::detensor_weight_one);
          nwa.addReturnTrans(q, *cit, high, qprime);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, high, qprime, DetensorWeight::detensor_weight_one);
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
        wts.setWeight(WeightGen::INTRA, q, low, qlow, new DetensorWeight(bdd_nithvar(bdd_level2var(con->baseRhsVocLevels[(4*con->numVars() - v - 1)/2]))));
      }else{
        vector< State > const& callStates = lTable[(4 * con->numVars() - v - 1)/2];
        for(vector< State >::const_iterator cit = callStates.begin(); cit != callStates.end(); ++cit){
          nwa.addReturnTrans(q, *cit, low, qlow);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, low, qlow, DetensorWeight::detensor_weight_one);
        }
      }
    }
    if(qhigh != rejectState){
      if(post_lowerplies(v)){
        nwa.addInternalTrans(q, high, qhigh);
        wts.setWeight(WeightGen::INTRA, q, high, qhigh, new DetensorWeight(bdd_ithvar(bdd_level2var(con->baseRhsVocLevels[(4*con->numVars() - v - 1)/2]))));
      }else{
        vector< State > const& callStates = lTable[(4 * con->numVars() - v - 1)/2];
        for(vector< State >::const_iterator cit = callStates.begin(); cit != callStates.end(); ++cit){
          nwa.addReturnTrans(q, *cit, high, qhigh);
          wts.setWeight(WeightGen::EXIT_TO_RET, q, high, qhigh, DetensorWeight::detensor_weight_one);
        }
      }
    }
  }
  return q;
}

bdd BinRel::reverse(bdd toReverse) const
{
#if defined(BINREL_STATS)
  con->numReverse++;
#endif
  //  bdd_fnprintdot_levels("a.dot", rel);
  //  bdd_fnprintdot_levels("b.dot", other);
  bdd rel1 = bdd_replace(toReverse, con->rawMove2Tensor2.get());
  //  bdd_fnprintdot_levels("amoveddown.dot", rel1);
  bdd rel2 = rel1 & con->t1OneBdd;
  //  bdd_fnprintdot_levels("amovedandeyed.dot", rel2);
  binrel_t halfwaythere = new BinRel(con, rel2);
  return halfwaythere->detensorViaNwa();

}
bdd BinRel::tensorViaDetensor(bdd other) const
{
  bdd rel3;
  if(other != con->cachedBaseOne->rel){
    rel3 = reverse(other);
  }else{
    rel3 = other;
  }
  bdd rel4 = bdd_replace(rel3, con->rawMove2Tensor2.get());  
  bdd rel5 = bdd_replace(rel, con->move2Tensor1.get());
  bdd c = rel4 & rel5;
  //  bdd_fnprintdot_levels("atensorb.dot", c);
  return c;
}
#endif //#ifdef NWA_DETENSOR

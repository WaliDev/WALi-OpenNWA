// ::wali::domains::binrel
#include "BinRel.hpp"
// ::opennwa
#include "opennwa/Nwa.hpp"

#ifndef NWA_DETENSOR
#error "Please define macro NWA_DETENSOR"
#endif
// //////////////////////////////////////////
// These functions provide a sneak-peek into the buddy bit-level
// variable ordering.

// Setup maps to enable quick lookup of information
void BddContext::setupLevelSets()
{
  tensor1LhsLevels.clear();
  tensor1RhsLevels.clear();
  tensor2LhsLevels.clear();
  tensor2RhsLevels.clear();

  for(BddContext::const_iterator cit = this->begin(); cit != this->end(); ++cit){
    int const * vars;
    bddinfo_t const varInfo = cit->second;
    //How many bits are used for me?
    if(varInfo->maxVal <= 0)
      continue;
    double fracnumbits = log2(varInfo->maxVal);
    int numbits = ceil(fracnumbits);

    vars = fdd_vars(varInfo->tensor1Lhs);
    assert(vars != NULL);
    for(int i=0; i < numbits; ++i)
      tensor1LhsLevels.insert(bdd_var2level(vars[i]));

    vars = fdd_vars(varInfo->tensor1Rhs);
    assert(vars != NULL);
    for(int i=0; i < numbits; ++i)
      tensor1RhsLevels.insert(bdd_var2level(vars[i]));

    vars = fdd_vars(varInfo->tensor2Lhs);
    assert(vars != NULL);
    for(int i=0; i < numbits; ++i)
      tensor2LhsLevels.insert(bdd_var2level(vars[i]));

    vars = fdd_vars(varInfo->tensor2Rhs);
    assert(vars != NULL);
    for(int i=0; i < numbits; ++i)
      tensor2RhsLevels.insert(bdd_var2level(vars[i]));
  }
  assert(tensor1LhsLevels.size() == tensor1RhsLevels.size());
  assert(tensor1RhsLevels.size() == tensor2LhsLevels.size());
  assert(tensor2LhsLevels.size() == tensor2RhsLevels.size());
}

unsigned BddContext::numVarsPerVoc()
{
  return tensor1LhsLevels.size();
}

bool BddContext::isRootInVocTensor1Lhs(bdd b)
{
  if(b == bddfalse || b == bddtrue) return false;
  return tensor1LhsLevels.find(bdd_var2level(bdd_var(b))) != tensor1LhsLevels.end();
}

bool BddContext::isRootInVocTensor2Lhs(bdd b)
{
  if(b == bddfalse || b == bddtrue) return false;
  return tensor2LhsLevels.find(bdd_var2level(bdd_var(b))) != tensor2LhsLevels.end();
}

bool BddContext::isRootInVocTensor1Rhs(bdd b)
{
  if(b == bddfalse || b == bddtrue) return false;
  return tensor1RhsLevels.find(bdd_var2level(bdd_var(b))) != tensor1RhsLevels.end();
}

bool BddContext::isRootInVocTensor2Rhs(bdd b)
{
  if(b == bddfalse || b == bddtrue) return false;
  return tensor2RhsLevels.find(bdd_var2level(bdd_var(b))) != tensor2RhsLevels.end();
}

bool BddContext::isRootRelevant(bdd b)
{
  if(b == bddfalse || b == bddtrue) return false;
  return isRootInVocTensor1Lhs(b) || isRootInVocTensor1Rhs(b) 
    || isRootInVocTensor2Lhs(b) || isRootInVocTensor2Rhs(b);
}




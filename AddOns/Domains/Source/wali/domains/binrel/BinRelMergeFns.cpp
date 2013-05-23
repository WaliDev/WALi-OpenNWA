#include "BinRelMergeFns.hpp"
#include <boost/cast.hpp>


using namespace std;
using namespace wali;
using namespace wali::domains;
using namespace wali::domains::binrel;

// //////////////////////////////////////////////////////
// Class MeetMergeFn
// //////////////////////////////////////////////////////
MeetMergeFn::MeetMergeFn(BddContext * con, std::vector<std::string> const& localVars, std::vector<std::string> const& localVars2)
{
  // start with Id in the base domain
  bdd havocCalleeLocalsBdd = con->getBaseOne()->getBdd();
  // start with top
  bdd constrainLocalsBdd = con->getBaseTop()->getBdd();
  for(std::vector<std::string>::const_iterator cit = localVars.begin(); cit != localVars.end(); ++cit){
    BddContext::const_iterator vocIter = con->find(*cit);
    if(vocIter == con->end()){
      cerr << "Unknown variable " << *cit << endl;
      assert(0);
    }
    // for each local variable:
    // havoc the post vocabulary for that variables
    havocCalleeLocalsBdd = bdd_exist(havocCalleeLocalsBdd, fdd_ithset(vocIter->second->baseRhs));
  }
  for (std::vector<std::string>::const_iterator cit2 = localVars2.begin(); cit2 != localVars2.end(); ++cit2){
    BddContext::const_iterator vocIter2 = con->find(*cit2);
    if (vocIter2 == con->end()){
      cerr << "Unknown variable " << *cit2 << endl;
      assert(0);
    }
    // enforce id across that variables
    constrainLocalsBdd = constrainLocalsBdd & bdd_biimp(fdd_ithset(vocIter2->second->baseLhs), fdd_ithset(vocIter2->second->baseRhs));
  }

  havocCalleeLocals = new BinRel(con, havocCalleeLocalsBdd);
  constrainLocals = new BinRel(con, constrainLocalsBdd);
}

bool MeetMergeFn::equal(merge_fn_t mf)
{
  MeetMergeFn * other = dynamic_cast<MeetMergeFn*>(mf.get_ptr());
  if(other == NULL)
    return false;
  return 
    (havocCalleeLocals == other->havocCalleeLocals) 
    && (constrainLocals == other->constrainLocals);
}

ostream& MeetMergeFn::print(ostream& o) const
{
  o << "MeetMergeFn [" << endl;
  havocCalleeLocals->print(o << "havocCalleeLocals: ") << endl;
  constrainLocals->print(o << "constrainLocals: ") << endl;
  o << "]";
  return o;
}


// Replaces the default merge fun: w1 x w2
sem_elem_t MeetMergeFn::apply_f(sem_elem_t w1, sem_elem_t w2)
{
  binrel_t b1 = boost::polymorphic_downcast<BinRel*>(w1.get_ptr());
  binrel_t b2 = boost::polymorphic_downcast<BinRel*>(w2.get_ptr());

  binrel_t ret;
  ret = b2 * havocCalleeLocals;
  ret = ret & constrainLocals;
  ret = b1 * ret;

  return ret;
}


// //////////////////////////////////////////////////////
// Class MeetMergeFn
// //////////////////////////////////////////////////////
TensorMergeFn::TensorMergeFn(BddContext * con, std::vector<std::string> const& localVars, std::vector<std::string> const& localVars2)
{
  id = con->getBaseOne();
  // start with Id in the base domain
  bdd havocCalleeLocalsBdd = con->getBaseOne()->getBdd();
  // start with Id in the tensor domain
  bdd idWithEqualLocalsBdd = con->getTensorOne()->getBdd();
  for(std::vector<std::string>::const_iterator cit = localVars.begin(); cit != localVars.end(); ++cit){
    BddContext::iterator vocIter = con->find(*cit);
    if(vocIter == con->end()){
      cerr << "Unknown variable " << *cit << endl;
      assert(0);
    }
    // for each local variable:
    // havoc the post vocabulary for that variables
    havocCalleeLocalsBdd = bdd_exist(havocCalleeLocalsBdd, fdd_ithset(vocIter->second->baseRhs));
  }
  for(std::vector<std::string>::const_iterator cit = localVars2.begin(); cit != localVars2.end(); ++ cit){
    BddContext::iterator vocIter = con->find(*cit);
    if(vocIter == con->end()){
      cerr << "Unkown variable " << *cit << endl;
      assert(0);
    }
    // enforce equality across the two tensor levels
    idWithEqualLocalsBdd = idWithEqualLocalsBdd & bdd_biimp(fdd_ithset(vocIter->second->tensor1Lhs), fdd_ithset(vocIter->second->tensor2Lhs));
  }

  havocCalleeLocals = new BinRel(con, havocCalleeLocalsBdd, false);
  idWithEqualLocals = new BinRel(con, idWithEqualLocalsBdd, true);
}

bool TensorMergeFn::equal(merge_fn_t mf)
{
  TensorMergeFn * other = dynamic_cast<TensorMergeFn*>(mf.get_ptr());
  if(other == NULL)
    return false;
  return
    havocCalleeLocals == other->havocCalleeLocals 
    && id == other->id 
    && idWithEqualLocals == other->idWithEqualLocals;
}

ostream& TensorMergeFn::print(ostream& o) const
{
  o << "TensorMergeFn [" << endl;
  havocCalleeLocals->print(o << "havocCalleeLocals: ") << endl;
  id->print(o << "id: ") << endl;
  idWithEqualLocals->print(o << "idWithEqualLocals: ") << endl;
  o << "]";
  return o;
}

// Replaces the default merge fun: w1 x w2
sem_elem_t TensorMergeFn::apply_f(sem_elem_t w1, sem_elem_t w2)
{
  sem_elem_tensor_t b1 = boost::polymorphic_downcast<SemElemTensor*>(w1.get_ptr());
  sem_elem_tensor_t b2 = boost::polymorphic_downcast<SemElemTensor*>(w2.get_ptr());

  sem_elem_tensor_t ret = b2;
  ret = boost::polymorphic_downcast<SemElemTensor*>(ret->extend(havocCalleeLocals.get_ptr()).get_ptr());
  ret = ret->tensor(id.get_ptr());
  ret = boost::polymorphic_downcast<SemElemTensor*>(idWithEqualLocals->extend(ret.get_ptr()).get_ptr());
  ret = ret->detensor();
  ret = boost::polymorphic_downcast<SemElemTensor*>(b1->extend(ret.get_ptr()).get_ptr());
  return ret;
}

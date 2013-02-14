#include "BinRelMergeFns.hpp"
#include <boost/cast.hpp>


using namespace std;
using namespace wali;
using namespace wali::cprover;
using namespace wali::domains;
using namespace wali::domains::binrel;

MeetMergeFn::MeetMergeFn(binrel_t hcl, binrel_t cl) :
  havocCalleeLocals(hcl),
  constrainLocals(cl) 
{}

bool MeetMergeFn::equal(merge_fn_t mf)
{
  MeetMergeFn * other = boost::polymorphic_downcast<MeetMergeFn*>(mf.get_ptr());
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

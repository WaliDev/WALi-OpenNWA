/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/Trans.hpp"
#include <iostream>
#include <sstream>

#define TRANS_COUNT_INSTANCES 1

namespace wali
{
  namespace wfa
  {


    int Trans::numTrans = 0;

    Trans::Trans() :
      kp(WALI_EPSILON,WALI_EPSILON), toStateKey(WALI_EPSILON),
      se(0),delta(0),status(MODIFIED),config(0)
    {
#if TRANS_COUNT_INSTANCES
      numTrans++;
      //*waliErr << "Trans(...) : " << numTrans << std::endl;
#endif
    }

    Trans::Trans(
        Key from_,
        Key stack_,
        Key to_,
        sem_elem_t se_ ) :
      kp(from_,stack_), toStateKey(to_),
      se(se_), delta(se_), status(MODIFIED), config(0) 
    {
#if TRANS_COUNT_INSTANCES
      numTrans++;
      //*waliErr << "Trans(...) : " << numTrans << std::endl;
#endif
    }

    //
    // Creating a new Trans means the delta this Trans
    // knows about is actually the se of param rhs
    //
    // There is a call to weight() to get the weight of rhs because
    // rhs may be a LazyTrans, which requires that the weight of a transition be
    // through a call to weight()
    //
    Trans::Trans( const Trans & rhs ) : IMarkable(), ITrans(), Markable()
    {
      this->operator=(rhs);
      { // DEBUGGING
        numTrans++;
        //*waliErr << "Trans( const Trans& ) : " << numTrans << std::endl;
      }
    }

    Trans::Trans( const ITrans & rhs ) : IMarkable(), ITrans(), Markable()
    {
      this->operator=(rhs);
      { // DEBUGGING
        numTrans++;
        //*waliErr << "Trans( const ITrans& ) : " << numTrans << std::endl;
      }
    }

    Trans &Trans::operator=(const Trans &rhs) {
      kp      = rhs.kp;
      toStateKey= rhs.toStateKey;
      se      = rhs.weight();
      delta   = rhs.weight();
      status  = rhs.status;
      config  = rhs.config;
      return *this;
    }

    Trans &Trans::operator=(const ITrans &rhs) {
      kp         = rhs.keypair();
      toStateKey = rhs.to();
      se         = rhs.weight();
      delta      = rhs.weight();
      status     = (rhs.modified()) ? MODIFIED : SAME;
      config     = rhs.getConfig();
      return *this;
    }

    Trans::~Trans()
    {
#if TRANS_COUNT_INSTANCES
      numTrans--;
      //*waliErr << "~Trans()   : " << numTrans << std::endl;
#endif
    }

    Trans* Trans::copy() const {
      return new Trans(from(),stack(),to(),weight());
    }

    Trans* Trans::copy(Key f, Key s, Key t) const {
      return new Trans(f,s,t,weight());
    }

    void Trans::combineTrans( ITrans* tp )
    {
      sem_elem_t wnew = tp->weight();

      // delta returns ( wnew + se, wnew - se )
      // Use w->delta(se) b/c we want the returned diff
      // to be what is in the new weight (wnew) and not
      // in the existing weight (se)
      std::pair< sem_elem_t , sem_elem_t > p = wnew->delta( se );

      // This's weight is w+se
      se = p.first;

      // Combine current delta with p's delta (wnew - se) and set status to
      // modified if this's delta changes value.
      sem_elem_t old_delta = delta;
      delta = delta->combine( p.second );
      status = ( old_delta->equal(delta) ) ? SAME : MODIFIED;
    }

    wpds::Config* Trans::getConfig() const {
      return config;
    }

    void Trans::setConfig( wpds::Config* c ) {
      config = c;
    }

    sem_elem_t Trans::poststar_eps_closure( sem_elem_t the_se ) {
      return weight()->extend(the_se);
    }

    TaggedWeight Trans::apply_post( TaggedWeight tw) const {
      return TaggedWeight(weight()->extend(tw.getWeight()), tw.getTag());
    }

    TaggedWeight Trans::apply_pre( TaggedWeight tw) const {
      return TaggedWeight(tw.getWeight()->extend(weight()), tw.getTag());
    }

    void Trans::applyWeightChanger(util::WeightChanger &wc) {
      sem_elem_t wt = wc.change_weight(weight());
      setWeight(wt);
    }

  } // namespace wfa

} // namespace wali


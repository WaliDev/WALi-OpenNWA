/**
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "wali/SemElem.hpp"

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

namespace wali
{

  const std::string SemElem::XMLTag("Weight");

  SemElem::SemElem() : Countable() {}

  std::ostream& SemElem::marshallWeight( std::ostream& o ) const
  {
    o << "<" << XMLTag << ">";
    marshall(o);
    o << "</" << XMLTag << ">";
    return o;
  }

  std::ostream& SemElem::marshall( std::ostream& o ) const
  {
    o << toString();
    return o;
  }

  bool
  SemElem::underApproximates(SemElem * that)
  {
    // a \sqsubset b   iff   a + b = b
    sem_elem_t combination = that->combine(this);
    return that->equal(combination);
  }

  /*
   * diff returns This - se
   * The default implementation justs lets the diff
   * be the combine of this and se if the comb is
   * not equal to this.  What that means is that if
   * "(this + se ) == this" then "this" already 
   * approximates the changes of se. The algorithm
   * stops the saturation process when all transitions'
   * deltas are zero.
   */
  sem_elem_t SemElem::diff( SemElem * se ATTR_UNUSED )
  {
    (void) se;
    *waliErr << "[ERROR] SemElem::diff must be overridden to be used.\n";
    assert(0);
    return 0;
  }

  sem_elem_t SemElem::quasi_one() const
  {
    return one();
  }

  //
  // @return < this + se, this - se >
  //
  std::pair< sem_elem_t , sem_elem_t > SemElem::delta( SemElem * se )
  {
    std::pair< sem_elem_t , sem_elem_t > rp;
    rp.first = combine(se);

    // Because we do not actually have a difference operator,
    // we simply see if the combine is equal to the passed in parameter.
    // If not, we say that our new "delta" or "difference" is the 
    // combined weight
    if( se->equal( rp.first ) ) {
      rp.second = zero();
    }
    else {
      assert(this->count >= 1); // try to make sure 'this' is already a refptr
      rp.second = rp.first;
    }
    return rp;
  }

  sem_elem_t SemElem::star() {
    sem_elem_t w = combine(one());
    sem_elem_t wn = w->extend(w);
    while(!w->equal(wn)) {
      w = wn;
      wn = wn->extend(wn);
    }
    return wn;
  }

  // computeDifference
  //
  //    the default implementation just returns d
  // 
  sem_elem_t SemElem::computeDifference(const sem_elem_t & se_prop, const sem_elem_t & d)
  {
    return d;
  }

  // extendAndCombineTransWeights
  // 
  //   wnew := delta extend this;
  //   < wnew + told, delta combine (wnew - told), !delta->equal(delta combine (wnew - told)) >
  //
  std::pair<std::pair<sem_elem_t, sem_elem_t>, bool> 
      SemElem::extendAndCombineTransWeights( 
                             const sem_elem_t & delta, 
                             const sem_elem_t & rweight, 
                             const sem_elem_t & se_old, 
                             const sem_elem_t & se_propagated,
                             const sem_elem_t & told,
                             const sem_elem_t & dold, bool existold )
  {
    sem_elem_t wnew = delta->extend(rweight);

    // delta returns ( wnew + told, wnew - told )
    // Use w->delta(told) b/c we want the returned diff
    // to be what is in the new weight (wnew) and not
    // in the existing weight (told)
    std::pair< sem_elem_t , sem_elem_t > p = wnew->delta( told );

    // This's weight is w+se
    sem_elem_t ans_se = p.first;

    // Combine current delta with p's delta (wnew - se) and set status to
    // modified if this's delta changes value.
    sem_elem_t old_delta = dold;
    sem_elem_t ans_delta = dold->combine( p.second );

    bool is_modified = ( !old_delta->equal(ans_delta) );

    std::pair<sem_elem_t, sem_elem_t> tmp(ans_se, ans_delta);
    return std::pair<std::pair<sem_elem_t, sem_elem_t>, bool>(tmp, is_modified);
  }

#if defined(__GNUC__)
  std::ostream &
  SemElem::print_typename(std::ostream & os) const
  {
    int out;
    char* demangled = abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, &out);
    assert(demangled);
    os << demangled;
    free(demangled);
    return os;
  }
#else
  std::ostream &
  SemElem::print_typename(std::ostream & os) const
  {
    os << typeid(*this).name();
    return os;
  }
#endif

  
  void test_semelem_impl(sem_elem_t x)
  {
    sem_elem_t z = x->zero();
    sem_elem_t o = x->one();

    bool good = true;

    if (z->equal(o) || o->equal(z) ) 
    {
      *waliErr << "[ERROR - test_semelem_impl] zero == one" << std::endl;
      good = false;
    }
    if (!z->equal(z))
    {
      *waliErr << "[ERROR - test_semelem_impl] zero != zero" << std::endl;
      good = false;
    }
    if (!o->equal(o))
    {
      *waliErr << "[ERROR - test_semelem_impl] one != one" << std::endl;
      good = false;
    }
    if (!x->equal(x))
    {
      *waliErr << "[ERROR - test_semelem_impl] x != x" << std::endl;
      good = false;
    }
    if (!o->equal(o->combine(z)))
    {
      *waliErr << "[ERROR - test_semelem_impl] one != (one + zero)" << std::endl;
      good = false;
    }
    if (!z->equal(o->extend(z)))
    {
      *waliErr << "[ERROR - test_semelem_impl] zero != (one * zero)" << std::endl;
      good = false;
    }
    if (!(z->extend(o)->equal( o->extend(z))))
    {
      *waliErr << "[ERROR - test_semelem_impl] zero*one != one*zero" << std::endl;
      good = false;
    }
    if (!(x->equal(x->combine(x))) )
    {
      *waliErr << "[ERROR - test_semelem_impl] x+x != x" << std::endl;
      good = false;
    }
    if (!x->equal( x->extend(o)) || !x->equal(o->extend(x)))
    {
      *waliErr << "[ERROR - test_semelem_impl] x != 1*x" << std::endl;
      good = false;
    }
    if (!z->underApproximates(z)) {
      *waliErr << "[ERROR - test_semelem_impl] 0 does not underapproximate 0" << std::endl;
      good = false;
    }
    if (!o->underApproximates(o)) {
      *waliErr << "[ERROR - test_semelem_impl] 1 does not underapproximate 1" << std::endl;
      good = false;
    }
    if (!z->underApproximates(o)) {
      *waliErr << "[ERROR - test_semelem_impl] 0 does not underapproximate 1" << std::endl;
      good = false;
    }
    if (!z->underApproximates(x)) {
      *waliErr << "[ERROR - test_semelem_impl] 0 does not underapproximate x" << std::endl;
      good = false;
    }
    if (o->underApproximates(z)) {
      *waliErr << "[ERROR - test_semelem_impl] 1 underapproximate 0" << std::endl;
      good = false;
    }
    if (x->underApproximates(z) && !x->equal(z)) {
      *waliErr << "[ERROR - test_semelem_impl] x underapproximates 0 (and x != 0)" << std::endl;
      good = false;
    }

    assert(good);
  }

} // namespace wali


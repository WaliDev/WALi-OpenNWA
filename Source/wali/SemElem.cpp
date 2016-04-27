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
  // There are two correct ways of implementing this - se for distributive domains:
  // (i) this + se = se? 0 : this + se
  // (ii) this + se = se? 0 : this
  //
  // This default implementation for delta uses option (i) since that approach is sound
  // even for non-distributive domains, ie, domains where x*y + x*z can be non-equal to x*(y+z)
  std::pair< sem_elem_t , sem_elem_t > SemElem::delta( SemElem * se )
  {
    std::pair< sem_elem_t , sem_elem_t > rp;

    // Use se as the first argument so that the old value se is the first argument to combine
    rp.first = se->combine(this);

    // Because we do not actually have a difference operator,
    // we simply see if the combine is equal to the passed in parameter.
    // If not, we say that our new "delta" or "difference" is the 
    // combined weight
    if( se->equal( rp.first ) ) {
      rp.second = zero();
    }
    else {
      rp.second = rp.first;
    }
    return rp;
  }

  sem_elem_t
  SemElem::extendAndDiff(sem_elem_t next, sem_elem_t subtrahend)
  {
    sem_elem_t ext = this->extend(next);
    std::pair<sem_elem_t , sem_elem_t> t = ext->delta(subtrahend);
    return t.second;
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


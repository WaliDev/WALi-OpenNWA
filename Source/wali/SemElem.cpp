/**
 * @author Nicholas Kidd
 * @version $Id: SemElem.cpp 1093 2011-06-07 16:50:18Z driscoll $
 */

#include "wali/SemElem.hpp"

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


  void test_semelem_impl(sem_elem_t x)
  {
    sem_elem_t z = x->zero();
    sem_elem_t o = x->one();
    if (z->equal(o) || o->equal(z) ) 
    {
      *waliErr << "[ERROR - test_semelem_impl] zero == one" << std::endl;
      assert(false);
    }
    if (!z->equal(z))
    {
      *waliErr << "[ERROR - test_semelem_impl] zero != zero" << std::endl;
      assert(false);
    }
    if (!o->equal(o))
    {
      *waliErr << "[ERROR - test_semelem_impl] one != one" << std::endl;
      assert(false);
    }
    if (!x->equal(x))
    {
      *waliErr << "[ERROR - test_semelem_impl] x != x" << std::endl;
      assert(false);
    }
    if (!o->equal(o->combine(z)))
    {
      *waliErr << "[ERROR - test_semelem_impl] one != (one + zero)" << std::endl;
      assert(false);
    }
    if (!z->equal(o->extend(z)))
    {
      *waliErr << "[ERROR - test_semelem_impl] zero != (one * zero)" << std::endl;
      assert(false);
    }
    if (!(z->extend(o)->equal( o->extend(z))))
    {
      *waliErr << "[ERROR - test_semelem_impl] zero*one != one*zero" << std::endl;
      assert(false);
    }
    if (!(x->equal(x->combine(x))) )
    {
      *waliErr << "[ERROR - test_semelem_impl] x+x != x" << std::endl;
      assert(false);
    }
    if (!x->equal( x->extend(o)) || !x->equal(o->extend(x)))
    {
      *waliErr << "[ERROR - test_semelem_impl] x != 1*x" << std::endl;
      assert(false);
    }

  }

} // namespace wali


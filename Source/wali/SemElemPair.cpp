/**
 * @author Nicholas Kidd
 * @version $Id: SemElemPair.cpp 513 2009-02-10 17:09:07Z kidd $
 */

#include "wali/SemElemPair.hpp"

namespace wali
{
  SemElemPair::SemElemPair( SemElem* f,SemElem* s ) :
    first(f),second(s) {}

  SemElemPair::SemElemPair( sem_elem_t f,sem_elem_t s ) :
    first(f),second(s) {}

  SemElemPair::~SemElemPair() {}

  // return the One element of the semiring
  sem_elem_t SemElemPair::one() const
  {
    return new SemElemPair( first->one(),second->one() );
  }

  // return the Zero element of the semiring
  sem_elem_t SemElemPair::zero() const
  {
    return new SemElemPair( first->zero(),second->zero() );
  }

  // Perform the extend operation
  sem_elem_t SemElemPair::extend( SemElem * se )
  {
    // TODO : Make dynamic checks debug only?
    //SemElemPair * that = static_cast< SemElemPair* >(se);
    SemElemPair * that = dynamic_cast< SemElemPair* >(se);
    assert( 0 != that );

    sem_elem_t fnew = first->extend(that->first);
    sem_elem_t snew = second->extend(that->second);
    return new SemElemPair(fnew,snew);
  }

  // Perform the combine operation
  sem_elem_t SemElemPair::combine( SemElem * se )
  {
    // TODO : Make dynamic checks debug only?
    //SemElemPair * that = static_cast< SemElemPair* >(se);
    SemElemPair * that = dynamic_cast< SemElemPair* >(se);
    assert( 0 != that );

    sem_elem_t fnew = first->combine(that->first);
    sem_elem_t snew = second->combine(that->second);
    return new SemElemPair(fnew,snew);
  }

  // Equality comparison between two semiring elements
  bool SemElemPair::equal( SemElem * se ) const
  {
    // TODO : Make dynamic checks debug only?
    //SemElemPair * that = static_cast< SemElemPair* >(se);
    SemElemPair * that = dynamic_cast< SemElemPair* >(se);
    assert( 0 != that );

    bool feq = first->equal( that->first );
    bool seq = second->equal( that->second );
    return feq && seq;
  }

  // Print the semiring element to the std::ostream o
  std::ostream& SemElemPair::print( std::ostream & o ) const
  {
    o << "SemElemPair< ";
    first->print(o);
    o << " , ";
    second->print(o);
    o << " >";
    return o;
  }

  // Perform the diff operation
  sem_elem_t SemElemPair::diff( SemElem * se )
  {
    // TODO : Make dynamic checks debug only?
    //SemElemPair * that = static_cast< SemElemPair* >(se);
    SemElemPair * that = dynamic_cast< SemElemPair* >(se);
    assert( 0 != that );

    return new SemElemPair(
        first->diff(that->first),
        second->diff(that->second) );
  }

  // Perform the quasi_one operation
  sem_elem_t SemElemPair::quasi_one() const
  {
    return new SemElemPair(first->quasi_one(), second->quasi_one());
  }

  // Perform the delta operation
  std::pair<sem_elem_t,sem_elem_t> SemElemPair::delta( SemElem * se )
  {
    // TODO : Make dynamic checks debug only?
    //SemElemPair * that = static_cast< SemElemPair* >(se);
    SemElemPair * that = dynamic_cast< SemElemPair* >(se);
    assert( 0 != that );

    std::pair<sem_elem_t, sem_elem_t> d1 = first->delta(that->first);
    std::pair<sem_elem_t, sem_elem_t> d2 = second->delta(that->second);
    std::pair<sem_elem_t, sem_elem_t> ret(
        new SemElemPair(d1.first.get_ptr(),d2.first.get_ptr()),
        new SemElemPair(d1.second.get_ptr(),d2.second.get_ptr())
        );
    return ret;
  }

} // namespace wali


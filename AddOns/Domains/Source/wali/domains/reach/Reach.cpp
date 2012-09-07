/**
 * @author Nicholas Kidd
 * @version $Id: Reach.cpp 485 2008-12-09 17:52:40Z kidd $
 */

#include "Reach.hpp"

using namespace wali::domains::reach;
using wali::sem_elem_t;
using wali::SemElem;

int Reach::numReaches = 0;

Reach::Reach( bool b ) :
  isreached(b)
{
  numReaches++;
  //std::cerr << "Reach(...) :" << numReaches << std::endl;
}

Reach::~Reach()
{
  numReaches--;
  //std::cerr << "~Reach()   :" << numReaches << std::endl;
}

sem_elem_t Reach::one() const
{
  static sem_elem_t O(new Reach(true));
  return O;
}

sem_elem_t Reach::zero() const
{
  static sem_elem_t Z(new Reach(false));
  return Z;
}

// zero is the annihilator for extend
sem_elem_t Reach::extend( SemElem* se )
{
  Reach* rhs = static_cast< Reach* >(se);
  return new Reach(isreached && rhs->isreached);
}

// zero is neutral for combine
sem_elem_t Reach::combine( SemElem* se )
{
  Reach* rhs = static_cast< Reach* >(se);
  return new Reach(isreached || rhs->isreached);
}

bool Reach::equal( SemElem* se ) const
{
  Reach* rhs = static_cast< Reach* >(se);
  return ( isreached == rhs->isreached );
}

std::ostream & Reach::print( std::ostream & o ) const
{
  return (isreached) ? o << "ONE" : o << "ZERO";
}

sem_elem_t Reach::getWeight( std::string s )
{
  if( s == "ONE" || s == "" ) {
    return one();
  }
  else {
    return zero();
  }
}

wali::merge_fn_t Reach::getMergeFn( std::string s )
{
  return new wali::MergeFn(getWeight(s));
}



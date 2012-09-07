/**
 * @author Nicholas Kidd
 * @version $Id: Reach.cpp 634 2009-12-28 17:16:20Z kidd $
 */

#include <iostream>
#include "Reach.hpp"

using std::cerr;
using std::cout;
using std::endl;

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
  return new Reach(true);
}

sem_elem_t Reach::zero() const
{
  return new Reach(false);
}

// zero is the annihilator for extend
sem_elem_t Reach::extend( SemElem* se )
{
  Reach* rhs = static_cast< Reach* >(se);
  if( isreached && rhs->isreached )
    return one();
  else // this and rhs are one()
    return zero();
}

// zero is neutral for combine
sem_elem_t Reach::combine( SemElem* se )
{
  Reach* rhs = static_cast< Reach* >(se);
  if( isreached || rhs->isreached )
    return one();
  else
    return zero();
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



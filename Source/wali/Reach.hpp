#ifndef REACH_GUARD
#define REACH_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/SemElem.hpp"

class Reach : public wali::SemElem
{

  public:

    Reach( bool b );

    virtual ~Reach();

    wali::sem_elem_t one() const;

    wali::sem_elem_t zero() const;

    // zero is the annihilator for extend
    wali::sem_elem_t extend( wali::SemElem* rhs );

    // zero is neutral for combine
    wali::sem_elem_t combine( wali::SemElem* rhs );

    bool equal( wali::SemElem* rhs ) const;

    std::ostream & print( std::ostream & o ) const;

    wali::sem_elem_t from_string( const std::string& s ) const;

    static int numReaches;

  protected:
    bool isreached;

};

#endif	// REACH_GUARD


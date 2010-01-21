#ifndef REACH_GUARD
#define REACH_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/MergeFnFactory.hpp"
#include <string>

using wali::SemElem;
using wali::sem_elem_t;

class Reach : 
  public wali::SemElem, 
  public wali::WeightFactory,
  public wali::MergeFnFactory
{

  public:
    static int numReaches;

  public:

    Reach( bool b );

    virtual ~Reach();

    sem_elem_t one() const;

    sem_elem_t zero() const;

    // zero is the annihilator for extend
    sem_elem_t extend( SemElem* rhs );

    // zero is neutral for combine
    sem_elem_t combine( SemElem* rhs );

    bool equal( SemElem* rhs ) const;

    std::ostream & print( std::ostream & o ) const;

    // For WeightFactory
    virtual sem_elem_t getWeight( std::string s );

    // For MergeFnFactory
    virtual wali::merge_fn_t getMergeFn( std::string s );

  protected:
    bool isreached;

};

#endif	// REACH_GUARD


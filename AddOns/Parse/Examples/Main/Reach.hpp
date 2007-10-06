#ifndef REACH_GUARD
#define REACH_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/SemElem.hpp"
#include "wali/WeightFactory.hpp"
#include <string>

using wali::SemElem;
using wali::sem_elem_t;

class Reach : public wali::SemElem, public wali::WeightFactory
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
        sem_elem_t getWeight( std::string& s );

    protected:
        bool isreached;

};

#endif	// REACH_GUARD

/*
 * $Log $
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


#ifndef MUST_GUARD
#define MUST_GUARD

#include "wali/SemElem.hpp"

class Must : public wali::SemElem
{
    public:
        static const int INFINITY;

        Must(int a,int b);

        virtual ~Must();

        bool equal( const Must& that ) const;

        //
        // SemElem methods
        //
        wali::sem_elem_t one() const;

        wali::sem_elem_t zero() const;

        wali::sem_elem_t extend( wali::SemElem * rhs );

        wali::sem_elem_t combine( wali::SemElem * rhs );

        bool equal( wali::SemElem * rhs ) const;

        std::ostream& print( std::ostream& o ) const;

    protected:
        int x;  //<! The net change in stack height
        int y;  //<! The deepest the stack got

};  // class Must

#endif MUST_GUARD

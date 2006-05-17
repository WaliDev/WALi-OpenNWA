#ifndef MAY_GUARD
#define MAY_GUARD

#include "wali/SemELem.hpp"

class May : public wali::SemElem
{
    public:
        static const int INFINITY;

        May(int a,int b);

        virtual ~May();

        bool equal( const May& that ) const;

        //
        // SemElem methods
        //
        wali::sem_elem_t one();

        wali::sem_elem_t zero();

        wali::sem_elem_t extend( wali::SemElem * rhs );

        wali::sem_elem_t combine( wali::SemElem * rhs );

        bool equal( wali::SemElem * rhs );

        std::ostream& print( std::ostream& o ) const;

    protected:
        int x;  //<! The net change in stack height
        int y;  //<! The deepest the stack got

};  // class May

#endif MAY_GUARD


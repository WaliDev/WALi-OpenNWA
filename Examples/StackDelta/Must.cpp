#include "Must.hpp"
#include <climits>

const int Must::INFINITY = INT_MAX;

Must::Must( int a,int b ) : x(a), y(b) {}

Must::~Must() {}

bool Must::equal( const Must& that ) const
{
    return x == that.x && y == that.y;
}

//
// SemElem methods
//
wali::sem_elem_t Must::one() const
{
    static wali::sem_elem_t ONE( new Must(0,0) );
    return ONE;
}

wali::sem_elem_t Must::zero() const
{
    static wali::sem_elem_t ZERO( new Must(INFINITY,INFINITY) );
    return ZERO;
}

wali::sem_elem_t Must::extend( wali::SemElem * rhs )
{
    Must* that = dynamic_cast< Must* >(rhs);
    that->print( print( std::cout << "\t" ) << " X " ) << std::endl;
    return new Must(x+that->x,std::max<int>(y,(x+that->y)));
}

wali::sem_elem_t Must::combine( wali::SemElem * rhs )
{
    Must* that = dynamic_cast< Must* >(rhs);
    if( x == INFINITY && y == INFINITY ) {
        return rhs;
    }
    else if( that->x == INFINITY && that->y == INFINITY ) {
        return this;
    }
    else {
        if( x != that->x ) {
            print( std::cout << "\tThis: " );
            that->print( std::cout << " + That: " ) << std::endl;
            assert( 0 );
        }
        return new Must(x,std::max<int>(y,that->y));
    }
}

bool Must::equal( wali::SemElem * rhs ) const
{
    return equal( *(dynamic_cast<Must*>(rhs)) );
}

std::ostream& Must::print( std::ostream& o ) const
{
    o << "Must< " << x << " , " << y << " >";
    return o;
}

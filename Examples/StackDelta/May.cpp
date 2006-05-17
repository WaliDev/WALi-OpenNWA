#include "May.hpp"
#include <climits>
#include <algorithm>

const int May::INFINITY = INT_MAX;

May::May( int a,int b ) : x(a), y(b) {}

May::~May() {}

bool May::equal( const May& that ) const
{
    return x == that.x && y == that.y;
}

//
// SemElem methods
//
wali::sem_elem_t May::one()
{
    static wali::sem_elem_t ONE( new May(0,0) );
    return ONE;
}

wali::sem_elem_t May::zero()
{
    static wali::sem_elem_t ZERO( new May(INFINITY,INFINITY) );
    return ZERO;
}

wali::sem_elem_t May::extend( wali::SemElem * rhs )
{
    May* that = dynamic_cast< May* >(rhs);
    return new May(x+that->x,std::max<int>(y,that->y));
}

wali::sem_elem_t May::combine( wali::SemElem * rhs )
{
    May* that = dynamic_cast< May* >(rhs);
    assert( x == that->x );
    return new May(x,std::min<int>(y,that->y));
}

bool May::equal( wali::SemElem * rhs ) const
{
    return equal( *(dynamic_cast<May*>(rhs)) );
}

std::ostream& May::print( std::ostream& o ) const
{
    o << "May< " << x << " , " << y << " >";
    return o;
}

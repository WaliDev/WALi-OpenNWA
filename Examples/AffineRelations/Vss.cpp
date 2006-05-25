/*!
 * @author Nick Kidd
 */

#include "Vss.hpp"
#include "ARConfig.h"
#include <fstream>
#include <sstream>

Vss::Vss( VSSemiring* impl_ ) : impl(impl_)
{
}

Vss::Vss(VectorSpace * vs, unsigned ui ) : impl( new VSSemiring(vs,ui) )
{
}

Vss::~Vss()
{
}

//---------------------------------
// semiring operations
//---------------------------------
sem_elem_t Vss::one() const
{
    return new Vss( impl->one() );
}

sem_elem_t Vss::zero() const
{
    return new Vss( impl->zero() );
}

sem_elem_t Vss::extend(SemElem * se)
{
    Vss* rhs = dynamic_cast<Vss*>(se);
    VSSemiring* ring = rhs->impl.get_ptr();
    return new Vss(impl->extend( ring ) );
}

sem_elem_t Vss::combine(SemElem * se)
{
    Vss* rhs = dynamic_cast<Vss*>(se);
    VSSemiring* ring = rhs->impl.get_ptr();
    return new Vss(impl->combine( ring ) );
}

bool Vss::equal(SemElem * se) const
{
    Vss* rhs = dynamic_cast<Vss*>(se);
    VSSemiring* ring = rhs->impl.get_ptr();
    return impl->equal( ring );
}

sem_elem_t Vss::diff(SemElem * se) const
{
    Vss* rhs = dynamic_cast<Vss*>(se);
    VSSemiring* ring = rhs->impl.get_ptr();
    return new Vss(impl->diff( ring ));
}

//------------------------------------
// output
//------------------------------------
std::ostream & Vss::print(std::ostream &o) const
{
    return impl->print(o);
}

VectorSpace * Vss::vs()
{
    return impl->vs();
}

//------------------------------------
// parsing
//------------------------------------
ModuleSpace *read_weight(std::istream &str)
{
    ModuleSpace *weight = new ModuleSpace(AR::dim);
    int mat[AR::dim * AR::dim];
    char c;
    int depth = 0;
OUTER:
    do {
        str >> c;
        if(str.eof()) {
            break;
        }
        else {
            if( c == '#' ) {
                break;
            }
            else if( c == '[' ) {
                for(int i = 0; i < AR::dim * AR::dim; i++) {
                    str >> mat[i];
                }
                str >> c; // it should be ']'
                assert( c == ']' );
                weight->insertMatrix(mat);
            }
            else {
                std::cerr << "ERROR?\n";
                return 0;
                //assert(0);
            }
        }
    } while(true);
    return weight;
} 

wali::sem_elem_t Vss::getWeight( std::string& s )
{
    std::stringstream ss(s);
    ModuleSpace* ms= read_weight(ss);
    if( 0 == ms ) {
        std::cerr << "Parsing s:\n\t" << s << "\n";
        assert(0);
    }
    return new Vss( new VSSemiring(ms) );
}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

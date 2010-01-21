/*!
 * @author Nick Kidd
 */

#include "ARSemElem.hpp"
#include "ARConfig.hpp"
#include <fstream>
#include <sstream>

namespace AR {
    ARSemElem::ARSemElem( ARSemiring* impl_ ) : impl(impl_)
    {
    }

    ARSemElem::ARSemElem(ModuleSpace * vs, unsigned ui ) : impl( new ARSemiring(vs,ui) )
    {
    }

    ARSemElem::~ARSemElem()
    {
    }

    //---------------------------------
    // semiring operations
    //---------------------------------
    sem_elem_t ARSemElem::one() const
    {
        return new ARSemElem( impl->one() );
    }

    sem_elem_t ARSemElem::zero() const
    {
        return new ARSemElem( impl->zero() );
    }

    sem_elem_t ARSemElem::extend(SemElem * se)
    {
        ARSemElem* rhs = static_cast<ARSemElem*>(se);
        ARSemiring* ring = rhs->impl.get_ptr();
        return new ARSemElem(impl->extend( ring ) );
    }

    sem_elem_t ARSemElem::combine(SemElem * se)
    {
        ARSemElem* rhs = static_cast<ARSemElem*>(se);
        ARSemiring* ring = rhs->impl.get_ptr();
        return new ARSemElem(impl->combine( ring ) );
    }

    bool ARSemElem::equal(SemElem * se) const
    {
        ARSemElem* rhs = static_cast<ARSemElem*>(se);
        ARSemiring* ring = rhs->impl.get_ptr();
        return impl->equal( ring );
    }

#if AR_SEMELEM_DIFF
    sem_elem_t ARSemElem::diff(SemElem * se) const
    {
        ARSemElem* rhs = static_cast<ARSemElem*>(se);
        ARSemiring* ring = rhs->impl.get_ptr();
        return new ARSemElem(impl->diff( ring ));
    }
#endif 

    //------------------------------------
    // output
    //------------------------------------
    std::ostream & ARSemElem::print(std::ostream &o) const
    {
        return impl->print(o);
    }

    ModuleSpace * ARSemElem::vs()
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
OUTER:
        do {
            str >> c;
            //std::cerr << c << std::endl;
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

    wali::sem_elem_t ARSemElem::getWeight( std::string& s )
    {
        std::stringstream ss(s);
        ModuleSpace* ms= read_weight(ss);
        if( 0 == ms ) {
            std::cerr << "Parsing s:\n\t" << s << "\n";
            assert(0);
        }
        return new ARSemElem( new ARSemiring(ms) );
    }

} // namespace AR

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */

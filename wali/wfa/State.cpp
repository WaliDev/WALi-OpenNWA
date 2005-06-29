/*!
 * $Id: State.cpp,v 1.1.1.1 2005/05/31 19:04:01 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.1.1.1 $
 */

#include "wali/wfa/State.hpp"

namespace wali
{
    namespace wfa
    {

        int State::numStates = 0;

        State::State( wali_key_t key, sem_elem_t W ) :
            name(key),se(W),delta(W)
        {
            assert( W.is_valid() );
            quasi = W->zero();
            {
                // TODO : R
                std::cerr << "State(...) : " << ++numStates << std::endl;;
            }
        }

        State::~State()
        {
            {
                // TODO : R
                std::cerr << "~State()   : " << --numStates << std::endl;;
            }
        }

        std::ostream & State::print( std::ostream & o ) const
        {
            o << "State[ name=\"" << name << "\" W=\"";
            o << se->to_string() << "\" ]";
            return o;
        }

        void State::add_trans( Trans * t )
        {
            // assert( t->from() == this->name );
            trans_ls.push_back( t );
        }
    }
}

/*
 * $Log: State.cpp,v $
 * Revision 1.1.1.1  2005/05/31 19:04:01  kidd
 * initial WALi import
 *
 */


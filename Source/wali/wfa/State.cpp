/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/State.hpp"

namespace wali
{
    namespace wfa
    {

        int State::numStates = 0;
        const std::string State::XMLTag("State");
        const std::string State::XMLInitialTag("initial");
        const std::string State::XMLFinalTag("final");
        const std::string State::XMLNameTag("Name");

        State::State( wali_key_t k, sem_elem_t W ) :
            Countable(true),key(k),se(W),delta_se(W)
        {
            assert( W.is_valid() );
            quasi = W->zero();
            {
                // TODO : R
                numStates++;
                //std::cerr << "State(...) : " << numStates << std::endl;;
            }
        }

        State::~State()
        {
            {
                // TODO : R
                numStates--;
                //std::cerr << "~State()   : " << --numStates << std::endl;;
            }
        }

        std::ostream & State::print( std::ostream & o ) const
        {
            o << "State[ name=\"" << name() << "\" W=\"";
            o << se->toString() << "\" ]";
            return o;
        }

        void State::add_trans( Trans * t )
        {
            // assert( t->from() == this->key );
            trans_ls.push_back( t );
        }

        void State::add_rev_trans( Trans * t )
        {
            rev_trans_ls.push_back(t);
        }

    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"

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
            { // BEGIN DEBUGGING
                // TODO : R
                numStates++;
                //std::cerr << "State(...) : " << numStates << std::endl;;
            } // END DEBUGGING
        }

        State::~State()
        {
            { // BEGIN DEBUGGING
                // TODO : R
                numStates--;
                //std::cerr << "~State()   : " << --numStates << std::endl;;
            } // END DEBUGGING
        }

        std::ostream & State::print( std::ostream & o ) const
        {
            o << "State[ name=\"" << name() << "\" W=\"";
            o << se->toString() << "\" ]";
            return o;
        }

        void State::addTrans( Trans * t )
        {
            transSet.insert(t);
        }

        bool State::eraseTrans(
                Key from,
                Key stack,
                Key to )
        {
            return transSet.erase(from,stack,to);
        }

        void State::clearTransSet()
        {
            transSet.clear();
        }

    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */

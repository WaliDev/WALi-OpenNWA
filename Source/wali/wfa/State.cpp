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

        State::State() :
            Countable(true),key(WALI_EPSILON),se(0),delta_se(0),quasi(0)
        {
            { // BEGIN DEBUGGING
                //std::cerr << "State() : " << numStates << std::endl;;
                numStates++;
            } // END DEBUGGING
        }

        State::State( Key k, sem_elem_t W ) :
            Countable(true),key(k),se(W),delta_se(W)
        {
            assert( W.is_valid() );
            quasi = W->zero();
            { // BEGIN DEBUGGING
                //std::cerr << "State(Key,sem_elem_t): " << numStates << std::endl;;
                numStates++;
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
            o << "State[ name=\"" << key2str(name()) << "\" W=\"";
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
            return (NULL != transSet.erase(from,stack,to));
        }

        bool State::eraseTrans( Trans* t )
        {
            return (NULL != transSet.erase(t));
        }

        void State::clearTransSet()
        {
            transSet.clear();
        }

        bool State::operator()( const State* a, const State* b ) const
        {
            return a->key < b->key;
        }

    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */

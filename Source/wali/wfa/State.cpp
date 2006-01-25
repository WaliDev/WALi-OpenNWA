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

        bool State::eraseTransFromForwardsList(
                Key from,
                Key stack,
                Key to )
        {
            return eraseTransFromList(from,stack,to,trans_ls);
        }

        bool State::eraseTransFromReverseList(
                Key from,
                Key stack,
                Key to )
        {
            return eraseTransFromList(from,stack,to,rev_trans_ls);
        }

        void State::clearTransLists()
        {
            trans_ls.clear();
            rev_trans_ls.clear();
        }

        bool State::eraseTransFromList(
                Key from,
                Key stack,
                Key to,
                trans_list_t& ls )
        {
            Trans terase(from,stack,to,0);
            bool eraseSuccess = false;
            iterator it = ls.begin();
            for( ; it != ls.end() ; it++ ) {
                Trans* t = *it;
                if( t->equal(terase) ) {
                    eraseSuccess = true;
                    ls.erase(it);
                    break;
                }
            }
            return eraseSuccess;
        }
    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */

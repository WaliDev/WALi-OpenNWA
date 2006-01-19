/*!
 * @author Nick Kidd
 */

#include <sstream>
#include <cassert>
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/Config.hpp"

namespace wali
{
    namespace wpds
    {

        int Rule::numRules = 0;

        const std::string Rule::XMLTag("Rule");
        const std::string Rule::XMLFromTag("from");
        const std::string Rule::XMLFromStackTag("fromStack");
        const std::string Rule::XMLToTag("to");
        const std::string Rule::XMLToStack1Tag("toStack1");
        const std::string Rule::XMLToStack2Tag("toStack2");

        Rule::Rule( Config *f_, Config *t_, wali_key_t stk2_, sem_elem_t se_ ) :
            f(f_),t(t_),stk2(stk2_),se(se_)
        {
            numRules++;
            //std::cerr << "Rule(...) : " << numRules << std::endl;
        }

        Rule::~Rule()
        {
            numRules--;
            //std::cerr << "~Rule()   : " << numRules << std::endl;
        }

        const Config & Rule::from() const { return *f; }

        /*!
         * @return from state key
         */
        wali_key_t Rule::from_state() const { return from().state(); }

        /*!
         * @return from stack key
         */
        wali_key_t Rule::from_stack() const { return from().stack(); }

        /*!
         * @return Config Rule transitions to
         */
        const Config & Rule::to() const { return *t; }

        /*!
         * @return to state key
         */
        wali_key_t Rule::to_state() const { return to().state(); }

        /*!
         * @return to stack key 1
         */
        wali_key_t Rule::to_stack1() const { return to().stack(); }

        /*!
         * @return to stack key 2
         */
        wali_key_t Rule::to_stack2() const { return stack2(); }

        /*!
         * @return const reference to this's Weight
         */
        const sem_elem_t& Rule::weight() const { return se; }

        /*! @return the Rule's weight */
        sem_elem_t Rule::weight() { return se; }
        
        /*! sets the weight of the Rule */
        void Rule::weight( sem_elem_t wnew )
        {
            se = wnew;
        }

        std::ostream & Rule::print( std::ostream &o ) const
        {
            o << "<";
            o << key2str(from_state());
            o << ", ";
            o << key2str(from_stack());
            o << "> -> <";
            o << key2str(to_state());
            o << ", ";
            wali_key_t stk1 = to_stack1();
            if( stk1 != WALI_EPSILON )
            {
                o << key2str(stk1);
                if( stk2 != WALI_EPSILON )
                {
                    o << " ";
                    o << key2str(to_stack2());
                }
            }
            else {
                // sanity check
                assert( WALI_EPSILON == to_stack2());
            }
            o << ">";
            o << "\t" << weight()->toString();
            return o;
        }

        std::ostream & Rule::marshall( std::ostream & o ) const
        {
            o << "<" << XMLTag << " ";
            o << XMLFromTag << "='" << key2str(from_state()) << "' ";
            o << XMLFromStackTag << "='" << key2str(from_stack()) << "' ";
            o << XMLToTag << "='" << key2str(to_state()) << "'";

            // Check optional stack symbols
            if( WALI_EPSILON != to_stack1() ) {
                o << " " << XMLToStack1Tag << "='" << key2str(to_stack1()) << "'";
                if( WALI_EPSILON != to_stack2() ) {
                    o << " " << XMLToStack2Tag << "='" << key2str(to_stack2()) << "'";
                }
            }
            else {
                // sanity check
                assert( WALI_EPSILON == to_stack2() );
            }
            o << ">";
            se->marshall(o);
            o << "</" << XMLTag << ">";
            return o;
        }

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

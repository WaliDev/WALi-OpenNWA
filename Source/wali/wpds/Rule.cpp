/*!
 * @author Nicholas Kidd
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
      //*waliErr << "Rule(...) : " << numRules << std::endl;
    }

    Rule::~Rule()
    {
      numRules--;
      //*waliErr << "~Rule()   : " << numRules << std::endl;
    }

    /*!
     * @brief Make a copy of the rule
     */
    void Rule::copy(const rule_t r) {
      assert(r.is_valid());
      f = r->f;
      t = r->t;
      stk2 = r->stk2;
      se = r->se;
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
    void Rule::setWeight( sem_elem_t wnew )
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
      se->marshallWeight(o);
      o << "</" << XMLTag << ">";
      return o;
    }
    
    // @author Amanda Burton
        /**
         *  TODO: fix comments
         *  operator== is needed by std::set
         */
        bool Rule::operator==( Rule other ) const
        {
          return ( (f->state() == other.f->state())
                    && (f->stack() == other.f->stack())
                    && (t->state() == other.t->state())
                    && (t->stack() == other.t->stack())
                    && (stk2 == other.stk2)
                    && (se == other.se) );
        }
        
        /**
         *  TODO: fix comments
         *  operator< is needed by std::set
         */
        bool Rule::operator<( Rule other ) const
        {
          if(f->state() == other.f->state())
          {
            if(f->stack() == other.f->stack())
            {
              if(t->state() == other.t->state())
              {
                if(t->stack() == other.t->stack())
                {
                  return stk2 < other.stk2;
                }
                else 
                  return t->stack() < other.t->stack();
              }
              else
                return t->state() < other.t->state();
            }
            else
              return f->stack() < other.f->stack();
          }
          else
            return f->state() < other.f->state();
        }

  } // namespace wpds

} // namespace wali


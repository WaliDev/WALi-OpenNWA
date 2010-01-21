#ifndef wali_wpds_RULE_GUARD
#define wali_wpds_RULE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/Countable.hpp"
#include "wali/Markable.hpp"
#include "wali/SemElem.hpp"
#include "wali/ref_ptr.hpp"

//
// begin namespace wali
//
namespace wali
{

  namespace wpds
  {

    class Config;
    class rule_t;

    namespace ewpds 
    {
      class EWPDS;
    }
    namespace fwpds
    {
      class FWPDS;
    }

    /*! @class Rule
     *
     * @see Config
     * @see sem_elem_t
     * @see wali_key_t
     * @see ref_ptr
     * @see rcmix
     */

    class Rule : public Printable, public Countable
    {

      public:
        friend class WPDS;
        friend class DebugWPDS;
        friend class ewpds::EWPDS;
        friend class fwpds::FWPDS;
        friend class RuleWitness;

        static const std::string XMLTag;
        static const std::string XMLFromTag;
        static const std::string XMLFromStackTag;
        static const std::string XMLToTag;
        static const std::string XMLToStack1Tag;
        static const std::string XMLToStack2Tag;

      public: // static methods

        /*!
         * Keep track of the number of Rule's that have
         * been allocated
         */
        static int numRules;

      public:

        /*!
         * @brief constructor
         */
        Rule( Config *f, Config *t, wali_key_t s2, sem_elem_t se  );

        /*!
         * @brief destructor
         */
        ~Rule();

        /*! @return Config Rule transitions from */
        const Config & from() const;// { return *f; }

        /*! @return from state key */
        wali_key_t from_state() const;// { return f->state(); }

        /*! @return from stack key */
        wali_key_t from_stack() const;// { return f->stack(); }

        /*! @return Config Rule transitions to */
        const Config & to() const;// { return *t; }

        /*! @return to state key */
        wali_key_t to_state() const;// { return to().state(); }

        /*! @return to stack key 1 */
        wali_key_t to_stack1() const;// { return to().stack(); }

        /*! @return to stack key 2 */
        wali_key_t to_stack2() const;// { return stack2(); }

        /*!
         * @return Rule rhs second stack symbol or WALI_EPSILON if no such
         * stack sym
         */
        wali_key_t stack2() const { return stk2; }

        /*! @return const reference to this's Weight */
        const sem_elem_t& weight() const;

        /*! @return the Rule's weight */
        sem_elem_t weight();

        /*!
         * Sets the weight on the Rule.
         *
         * @param wnew the new weight for the rule
         * @return void
         */
        void setWeight( sem_elem_t wnew );

        /*!
         * overrides (implements) Printable::print method
         *
         * @param o the std::ostream this is written to
         * @return std::ostream passed in
         *
         * @see Printable
         */
        virtual std::ostream & print( std::ostream &o ) const;

        /*!
         * marshall will write a Rule to a std::ostream. This writes
         * the rule in XML form.
         *
         * @return std::ostream the marshalled rule was written to.
         */
        std::ostream& marshall( std::ostream & o ) const;

        /*!
         * @return true if this rule has 2 rhs stack symbols
         */
        bool is_rule2() const { return (stack2() != WALI_EPSILON); }

        // @author Amanda Burton
        /**
         *  TODO: fix comments
         *  operator== is needed by std::set
         */
        bool operator==( Rule other ) const;
        
        /**
         *  TODO: fix comments
         *  operator< is needed by std::set
         */
        bool operator<( Rule other ) const;

        /*!
         * @brief Make a copy of the rule
         */
        virtual void copy(const rule_t r);

      protected:

        Config * from() { return f; }
        Config * to() { return t; }

      protected:
        Config *f;
        Config *t;
        wali_key_t stk2;
        sem_elem_t se;

    };

    /*! @class rule_t
     *
     * Wrapper class for ref_ptr that ensures type safety.
     */
    class rule_t {

      public:

        rule_t() : rc(0) {}

        rule_t( Rule * ptr ) : rc(ptr) {}

        rule_t( const rule_t & rhs ) : rc( rhs.rc ) {}

        Rule * operator->() {
          return get_ptr();
        }

        Rule & operator*() {
          return *get_ptr();
        }

        rule_t & operator=( const rule_t &rhs ) {
          rc = rhs.rc;
          return *this;
        }

        rule_t & operator=( Rule * r ) {
          rc = r;
          return *this;
        }

        Rule * get_ptr() {
          return (Rule *)rc.get_ptr();
        }

        //
        // const methods
        //
        const Rule * operator->() const {
          return get_ptr();
        }

        const Rule & operator*() const {
          return *get_ptr();
        }

        const Rule * get_ptr() const {
          return (const Rule *)rc.get_ptr();
        }

        bool is_empty() const {
          return rc.is_empty();
        }

        bool is_valid() const {
          return rc.is_valid();
        }

      private:

        ref_ptr< Rule > rc;
    };

  } // end namespace wpds

} // end namespace wali

#endif  // wali_wpds_RULE_GUARD



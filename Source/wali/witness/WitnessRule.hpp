#ifndef wali_wpds_WITNESS_RULE_GUARD
#define wali_wpds_WITNESS_RULE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/wpds/Rule.hpp"

namespace wali
{
  using wpds::Rule;
  using wpds::rule_t;

  namespace witness
  {
    struct RuleStub : public Printable
    {
      RuleStub( const Rule& r );

      Key from_state();

      Key from_stack();

      Key to_state();

      Key to_stack1();

      Key to_stack2();

      sem_elem_t weight();

      std::ostream& print( std::ostream& o ) const;

      Key fs,fstk,ts,tstk1,tstk2;
      sem_elem_t se;
    };

    /*!
     * @class WitnessRule
     *
     * This class bears witness to the fact that a rule was created with
     * the supplied weight.
     */
    class WitnessRule : public Witness
    {
      public:
        /*!
         * @brief constructor creates a RuleStub from a
         * wali::wpds::Rule.
         *
         * @see Witness
         * @see Rule
         */
        WitnessRule( const Rule& r );

        //! Destructor does nothing.
        ~WitnessRule();

        //! Override Witness::accept
        virtual void accept( Visitor& v, bool visitOnce=false );

        //! Overrides Witness::prettyPrint
        virtual std::ostream& prettyPrint( std::ostream& o,size_t depth ) const;

        RuleStub& getRuleStub();

      protected:
        RuleStub stub;
    };

  } // namespace witness

} // namespace wali

#endif	// wali_wpds_WITNESS_RULE_GUARD


#ifndef wali_wpds_WITNESS_RULE_GUARD
#define wali_wpds_WITNESS_RULE_GUARD 1

/*!
 * @author Nick Kidd
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
        class RuleStub : public Printable
        {
            public:
                RuleStub( const Rule& r );

                Key from_state();

                Key from_stack();

                Key to_state();

                Key to_stack1();

                Key to_stack2();

                sem_elem_t weight();

                std::ostream& print( std::ostream& o ) const;

            protected:
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
                 * @brief WitnessRule constructor.
                 *
                 * The constructor copies the underlying Rule object contained
                 * within parameter rule_t. The weight on the member variable
                 * Rule r will be null. Reason being that the user supplied
                 * weight is the member variable se of the parent class
                 * Witness.
                 *
                 * @see Witness
                 * @see Rule
                 *
                 */
                WitnessRule( const Rule& r );

                /*!
                 * Destructor does nothing.
                 */
                ~WitnessRule();

                /*!
                 * Overrides Witness::pretty_pring
                 */
                virtual std::ostream& pretty_print( std::ostream& o,size_t depth ) const;

                RuleStub& rule_stub();

            protected:
                RuleStub stub;
        };

    } // namespace witness

} // namespace wali

#endif	// wali_wpds_WITNESS_RULE_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

#ifndef wali_wpds_WITNESS_RULE_GUARD
#define wali_wpds_WITNESS_RULE_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/witness/Witness.hpp"
#include "wali/wpds/Rule.hpp"

namespace wali
{
    using wpds::Rule;
    using wpds::rule_t;

    namespace witness
    {
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
                WitnessRule( rule_t r );

                /*!
                 * Destructor does nothing.
                 */
                ~WitnessRule();

                /*!
                 * Overrides Witness::pretty_pring
                 */
                virtual std::ostream& pretty_print( std::ostream& o,size_t depth ) const;

                const Rule& rule() const;

            protected:
                Rule r;
        };

    } // namespace witness

} // namespace wali

#endif	// wali_wpds_WITNESS_RULE_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

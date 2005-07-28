#ifndef wali_wpds_WITNESS_WPDS_GUARD
#define wali_wpds_WITNESS_WPDS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/wpds/WPDS.hpp"

namespace wali
{
    class SemElem;
    class Worklist;

    namespace wpds
    {
        class WitnessTransCopyLinker;

        /*!
         * @class WitnessWPDS
         */
        class WitnessWPDS : public WPDS
        {
            public:
                friend class WitnessTransCopyLinker;
                using WPDS::add_rule;

            public:
                /*!
                 * @brief Constructor passes wl to WPDS
                 */
                WitnessWPDS( Worklist * wl = 0 );

                /*!
                 * @brief Destructor does nothing
                 */
                virtual ~WitnessWPDS();

                /*!
                 * Override WPDS::add_rule so that the weight on each rule is
                 * a WitnessRule.
                 *
                 * @return true if rule existed
                 *
                 * @see sem_elem_t
                 * @see wali_key_t
                 */
                virtual bool add_rule(
                        wali_key_t from_state,
                        wali_key_t from_stack,
                        wali_key_t to_state,
                        wali_key_t to_stack1,
                        wali_key_t to_stack2,
                        sem_elem_t se );

                /*!
                 * link input WFA transitions to Configs overrides
                 * WPDS::copy_and_link so that a TransWitnesses are being
                 * created as weights on the LinkedTrans
                 *
                 * @see WFA
                 * @see LinkedTrans
                */
                virtual void copy_and_link( const WFA & in, WFA & dest );

            protected:

        }; // class WitnessWPDS

    } // namespace wpds

} // namespace wali

#endif	// wali_wpds_WITNESS_WPDS_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

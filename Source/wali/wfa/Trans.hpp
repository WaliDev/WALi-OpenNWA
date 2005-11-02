#ifndef wali_wfa_TRANS_GUARD
#define wali_wfa_TRANS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include <iostream>
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/Countable.hpp"
#include "wali/Markable.hpp"
#include "wali/SemElem.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/ref_ptr.hpp"

namespace wali
{
    namespace wpds
    {
        class WPDS;
        namespace ewpds
        {
            class EWPDS;
        }
    }

    namespace wfa
    {

        class WFA;
        class Trans;

        /*!
         * @typedef trans_t
         * Avoid typing ref_ptr<Trans> when reference counting is begin
         * enforced.
         */
        typedef ref_ptr<Trans> trans_t;

        /*!
         * @class Trans
         *
         * A Trans is a 4-tuple of from state, stack, to state, and Weight. It
         * represents a transition in the WFA.
         *
         * Markable is to make a Trans able to be placed in a Worklist.
         * Countable is for reference counting.
         *
         * @see Printable
         * @see Countable
         * @see Markable
         * @see Worklist
         * @see WFA 
         * @see sem_elem_t
         * @see ref_ptr
         */

        class Trans : public Printable, public Countable, public Markable
        {
            //
            // Types
            //
            public:
                friend class WFA;
                friend class ::wali::wpds::WPDS;
                friend class ::wali::wpds::ewpds::EWPDS;
                enum status_t { MODIFIED,SAME };
                static int numTrans;
                static const std::string XMLTag;
                static const std::string XMLFromTag;
                static const std::string XMLStackTag;
                static const std::string XMLToTag;

            protected:
            private:

                //
                // Methods
                //
            public:
                /*!
                 * Construct an empty Trans
                 */
                Trans();

                /*!
                 * @brief constructor
                 *
                 * This is used by CA when add_trans is called.
                 */
                Trans(  wali_key_t from,
                        wali_key_t stack,
                        wali_key_t to,
                        sem_elem_t se );

                Trans( const Trans & t );

                virtual ~Trans();

                //
                // getters (const)
                //
                /*! @return const wali_key_t of from state */
                const wali_key_t from() const throw() {
                    return kp.first;
                }

                /*! @return const wali_key_t of from state */
                const wali_key_t from_state() const throw() {
                    return kp.first;
                }

                /*! @return const wali_key_t of stack symbol */
                const wali_key_t stack() const throw() {
                    return kp.second;
                }

                /*! @return const wali_key_t of to state */
                const wali_key_t to() const throw() {
                    return t;
                }

                /*! @return const wali_key_t of to state */
                const wali_key_t to_state() const throw() {
                    return t;
                }

                /*! @return const sem_elem_t of Trans */
                const sem_elem_t weight() const throw() {
                    return se;
                }

                //
                // getters (non const)
                //
                /*! @return wali_key_t of from state */
                wali_key_t from() throw() {
                    return kp.first;
                }

                /*! @return wali_key_t of from state */
                wali_key_t from_state()throw() {
                    return kp.first;
                }

                /*! @return wali_key_t of stack symbol */
                wali_key_t stack() throw() {
                    return kp.second;
                }

                /*! @return wali_key_t of to state */
                wali_key_t to() throw() {
                    return t;
                }

                /*! @return wali_key_t of to state */
                wali_key_t to_state() throw() {
                    return t;
                }

                /*! @return sem_elem_t of Trans */
                sem_elem_t weight() throw() {
                    return se;
                }

                //
                // setters
                //
                /*!
                 * Sets the weight of the Trans to be param w.
                 * The Trans's status will be set to MODIFIED if
                 * param w != this->weight()
                 *
                 * @param w the sem_elem_t for the new weight
                 */
                void weight( sem_elem_t w );
                void set_weight( sem_elem_t w );
                void combine_weight( sem_elem_t w );

                /*! @return true if param rhs is equal to this */
                virtual bool equal( const trans_t & rhs ) const;

                /*! @return true if param rhs is equal to this */
                virtual bool equal( const Trans & rhs ) const;

                /*! @return true if param rhs is equal to this */
                virtual bool equal( const Trans * rhs ) const;

                /*!
                 * Print this transition to the ostream. Overrides
                 * or implements Printable::print
                 *
                 * @return std::ostream this was written to
                 *
                 * @see Printable
                 *  
                 */
                virtual std::ostream& print( std::ostream& o ) const;

                /*!
                 * @brief Print Trans in XML to ostream o
                 *
                 * @return std::ostream the marshalled Trans was written to.
                 */
                virtual std::ostream& marshall( std::ostream& o ) const;

                /*!
                 * Return const referenct to this transitions KeyPair
                 *
                 * The KeyPair holds the from state and stack symbol.
                 *
                 * @see KeyPair
                 * @return const KeyPair reference
                 */
                const KeyPair & keypair() const throw()
                {
                    return kp;
                }

                /*!
                 * @brief return true if the transition has been modified
                 *
                 * A Trans is considered modified if when its weight changes. This
                 * includes the initial creation of a Trans object. This follows
                 * from the fact that all Trans can be considered (abstractly)
                 * created with a weight of ZERO
                 *
                 * @return true if this transition has been modified
                 */
                bool modified() const throw()
                {
                    return (status == MODIFIED);
                }

            protected:
                KeyPair kp;
                wali_key_t t;
                sem_elem_t se;
                sem_elem_t delta;

            protected:  // vars used in Process and not relevant to Trans
                status_t status;
        };

    } // namespace wfa

} // namespace wali

#endif  // wali_wfa_TRANS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

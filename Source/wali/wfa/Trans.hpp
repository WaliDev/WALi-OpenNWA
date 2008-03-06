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
    namespace wfa
    {

        class WFA;
        class Trans;

        /*!
         * @struct TransLT
         *
         * This can be used to place Trans* into std::set or any other STL
         * container that uses Strict-weak ordering.
         */
        struct TransLT {
            bool operator()( const Trans* a, const Trans* b ) const;
        };

        struct TransEq {
            bool operator()( const Trans* a, const Trans* b ) const;
        };

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
                 * This is used by WFA when addTrans is called.
                 */
                Trans(  Key from,
                        Key stack,
                        Key to,
                        sem_elem_t se );

                Trans( const Trans & t );
                const Trans &operator =(const Trans &t);

                virtual ~Trans();

                //
                // getters (const)
                //
                /*! @return const Key of from state */
                const Key from() const throw() {
                    return kp.first;
                }

                /*! @return const Key of from state */
                const Key from_state() const throw() {
                    return kp.first;
                }

                /*! @return const Key of stack symbol */
                const Key stack() const throw() {
                    return kp.second;
                }

                /*! @return const Key of to state */
                const Key to() const throw() {
                    return toStateKey;
                }

                /*! @return const Key of to state */
                const Key to_state() const throw() {
                    return toStateKey;
                }

                /*! @return const sem_elem_t of Trans */
                virtual const sem_elem_t weight() const throw() {
                    return se;
                }

                /*!
                 * @return const sem_elem_t delta of Trans
                 *
                 * delta is used in computing fixpoints
                 */
                const sem_elem_t getDelta() const throw() {
                    return delta;
                }

                //
                // getters (non const)
                //
                /*! @return Key of from state */
                Key from() throw() {
                    return kp.first;
                }

                /*! @return Key of from state */
                Key from_state()throw() {
                    return kp.first;
                }

                /*! @return Key of stack symbol */
                Key stack() throw() {
                    return kp.second;
                }

                /*! @return Key of to state */
                Key to() throw() {
                    return toStateKey;
                }

                /*! @return Key of to state */
                Key to_state() throw() {
                    return toStateKey;
                }

                /*! @return sem_elem_t of Trans */
                virtual sem_elem_t weight() throw() {
                    return se;
                }

                /*!
                 * @return sem_elem_t delta of Trans
                 *
                 * delta is used in computing fixpoints
                 */
                sem_elem_t getDelta() throw() {
                    return delta;
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
                virtual void setWeight( sem_elem_t w ) {
                    se = w;
                }

                /*!
                 * Set the delta value for the Trans.
                 */
                void setDelta( const sem_elem_t w ) {
                    delta = w;
                }

                virtual void combine_weight( sem_elem_t w );

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
                Key toStateKey;
                mutable sem_elem_t se;
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

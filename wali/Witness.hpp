#ifndef wali_WITNESS_GUARD
#define wali_WITNESS_GUARD 1

/*!
 * @author Nick Kidd
 * @version $Revision: 1.6 $
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"

namespace wali
{
    class Witness;
    typedef ref_ptr< Witness > witness_t;

    /*!
     * @class Witness
     */
    class Witness : public SemElem
    {
        public:
            Witness( sem_elem_t se );

            virtual ~Witness();

            /*!
             */
            virtual sem_elem_t one() const;

            /*!
             */
            virtual sem_elem_t zero() const;

            /*!
             * Extend the user supplied weight and create a new Witness
             */
            virtual sem_elem_t extend( SemElem * se );

            /*!
             */
            virtual sem_elem_t combine( SemElem * se );

            /*!
             * Test for equality of user supplied weight. The Witness is
             * simply along for the ride.
             */
            virtual bool equal( SemElem * se ) const;

            /*!
             * Print the Witness to the parameter o. 
             */
            virtual std::ostream& print( std::ostream& o ) const;

            /*!
             * Pretty print in a DAG like structure to the ostream
             *
             * TODO : The ability to output in dotty format would be nice
             */
            virtual std::ostream& pretty_print( std::ostream& o, size_t depth ) const;

            /*!
             * Prints "  |" to parameter o for each level of depth
             *
             * TODO : should "  |" be a macro, a configurable string, or a
             *      parameter to format_depth?
             */
            std::ostream& format_depth( std::ostream& o, size_t depth ) const;

            /*!
             * Returns protected sem_elem_t se member variable that is the
             * user supplied weight or from a computation involving the
             * user defined semiring.
             *
             * @see sem_elem_t
             */
            sem_elem_t weight() { return user_se; }

        protected:
            sem_elem_t user_se;

    }; // class Witness

} // namespace wali

#endif  // wali_WITNESS_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


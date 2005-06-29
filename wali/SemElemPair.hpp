#ifndef wali_SEMELEM_PAIR_GUARD
#define wali_SEMELEM_PAIR_GUARD 1

/*!
 * @author Nick Kidd
 * @version $Revision: 1.6 $
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"

namespace wali
{
    class SemElemPair : public SemElem
    {
        public:
            SemElemPair( SemElem* fst, SemElem* snd );

            SemElemPair( sem_elem_t fst, sem_elem_t snd );

            virtual ~SemElemPair();

            //!
            //! return the One element of the semiring
            //!
            virtual sem_elem_t one() const;

            //!
            //! return the Zero element of the semiring
            //!
            virtual sem_elem_t zero() const;

            //!
            //! Perform the extend operation
            //!
            virtual sem_elem_t extend( SemElem * se );

            //!
            //! Perform the combine operation
            //!
            virtual sem_elem_t combine( SemElem * se );

            //!
            //! Equality comparison between two semiring elements
            //!
            virtual bool equal( SemElem * se ) const;

            //!
            //! Print the semiring element to the std::ostream o
            //!
            virtual std::ostream& print( std::ostream & o ) const;

            //!
            //! Perform the diff operation
            //!
            virtual sem_elem_t diff( SemElem * se );

            //!
            //! Perform the quasi_one operation
            //!
            virtual sem_elem_t quasi_one() const;

            //!
            //! Perform the delta operation
            //!
            virtual std::pair<sem_elem_t,sem_elem_t> delta( SemElem * se );

            //!
            //! return first element
            //!
            sem_elem_t get_first() const {
                return first; 
            }

            //!
            //! return second element
            //!
            sem_elem_t get_second() const {
                return second; 
            } 

        protected:
            sem_elem_t first;
            sem_elem_t second;

    }; // class SemElemPair

} // namespace

#endif  // wali_SEMELEM_PAIR_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


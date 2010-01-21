#ifndef wali_WITNESS_GUARD
#define wali_WITNESS_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Markable.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"
#include "wali/witness/Visitable.hpp"

namespace wali
{
  class Visitor;

  namespace witness
  {
    class Witness;

    struct witness_t : public wali::ref_ptr< Witness > 
    {
      typedef wali::ref_ptr< Witness > Parent;
      witness_t();
      witness_t( sem_elem_t );
      witness_t( Witness* );
      witness_t& operator=( sem_elem_t se );
      witness_t& operator=( Witness* alpha );
      Witness* getWitness( sem_elem_t se );
    };

    /**
     * @class Witness
     */
    class Witness : public SemElem, public Markable, public Visitable
    {
      public:
        static int COUNT;

      public:
        Witness( sem_elem_t se );

        virtual ~Witness();

        /*!
         * Test if the Witness has user weight ZERO
         */
        bool isZero();

        /*!
         * Test if the Witness has user weight ONE
         */
        bool isOne();

        /*!
         * Returns a new Witness whose user_se is a sem_elem_t ONE
         * element of the user's weight domain.
         *
         * @see SemElem
         * @see sem_elem_t
         */
        virtual sem_elem_t one() const;

        /*!
         * Returns a new Witness whose user_se is a sem_elem_t ZERO
         * element of the user's weight domain.
         *
         * @see SemElem
         * @see sem_elem_t
         */
        virtual sem_elem_t zero() const;

        /*!
         * Extend the user supplied weight and create a new WitnessExtend.
         *
         * @see WitnessExtend
         */
        virtual sem_elem_t extend( SemElem * se );

        /*!
         * Combines the user's weight and creates a WitnessCombine
         * if parameter "se" is not already a WitnessCombine.
         *
         * @see SemElem
         * @see sem_elem_t
         * @see WitnessCombine
         */
        virtual sem_elem_t combine( SemElem * se );

        /*!
         * Test for equality of user supplied weight. The Witness is
         * simply along for the ride.
         */
        virtual bool equal( SemElem * se ) const;

        //! Print the Witness to the parameter o. 
        virtual std::ostream& print( std::ostream& o ) const;

        /*!
         * Accept method allows for different behaviors to be applied to
         * the Witness DAG.
         *
         * @see Visitor
         */
        virtual void accept( Visitor& v, bool visitOnce=false );

        //! Pretty print in a DAG like structure to the ostream
        virtual std::ostream& prettyPrint( std::ostream& o, size_t depth ) const;

        //! Prints "  |" to parameter o for each level of depth
        std::ostream& formatDepth( std::ostream& o, size_t depth ) const;

        /*!
         * Returns protected sem_elem_t se member variable that is the
         * user supplied weight or from a computation involving the
         * user defined semiring.
         *
         * @see sem_elem_t
         */
        sem_elem_t weight() { return user_se; }

        //! Reset all marks (recursively)
        virtual void reset_marks() const;

      protected:
        sem_elem_t user_se; //!< The user SemElem value
        bool isEmpty;       //!< True if type == Witness. Cheap reflection.

      private:
        Witness( sem_elem_t se, bool ie );

    }; // class Witness

  } // namespace witness

} // namespace wali

#endif  // wali_WITNESS_GUARD


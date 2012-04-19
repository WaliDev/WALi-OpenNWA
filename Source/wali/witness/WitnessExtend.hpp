#ifndef wali_WITNESS_EXTEND_GUARD
#define wali_WITNESS_EXTEND_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/Witness.hpp"

namespace wali
{
  namespace witness
  {
    /*!
     * @class WitnessExtend
     *
     * Class WitnessExtend bears witness to the fact that a new weight
     * (sem_elem_t) was created by extending 0, 1, or 2 weights. In the normal
     * case it will be extend of two weights. It really does not make sense
     * for there to be an extend of 0 weights.
     *
     * @see Witness
     * @see sem_elem_t
     */
    class WitnessExtend : public Witness
    {
      public:
        /*!
         * Constructor creates WitnessExtend with no children.
         * How does this happen?
         */
        //WitnessExtend( sem_elem_t weight );

        /*!
         * Constructor creates WitnessExtend with only a left child.
         * Parameter weight should equal left
         */
        //WitnessExtend( sem_elem_t weight, witness_t left );

        /*!
         * Constructor creates WitnessExtend with a left & right child.
         * Parameter weight should equal left.extend(right)
         */
        WitnessExtend( sem_elem_t weight, witness_t left, witness_t right );

        //! Destructor does nothing.
        virtual ~WitnessExtend();

        //! Override Witness::accept
        virtual void accept( Visitor& v, bool visitOnce=false );

        //! Override Witness::reset_marks
        virtual void reset_marks() const;

        //! Override Witness::pretty_print
        virtual std::ostream& prettyPrint( std::ostream& o, size_t depth ) const;

        //! @return witness_t that is the left child
        witness_t left() { return lchild; }

        //! @return true if the left child is valid
        bool hasLeft() const { return !lchild.is_empty(); }

        //! @return witness_t that is the right child
        witness_t right() { return rchild; }

        //! @return true if the right child is valid
        bool hasRight() const { return !rchild.is_empty(); }

      protected:
        witness_t lchild;      //! < left parameter of binary extend
        witness_t rchild;      //! < right parameter of binary extend

    }; // class WitnessExtend

  } // namespace witness

} // namespace wali

#endif  // wali_WITNESS_EXTEND_GUARD


#ifndef wali_witness_WITNESS_MERGE_GUARD
#define wali_witness_WITNESS_MERGE_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessMergeFn.hpp"

namespace wali {

  namespace witness {

    class WitnessMerge : public Witness {

      public:
        //! Constructor bears witness to a merge function being
        //! applied to left and right with result weight
        WitnessMerge(
            sem_elem_t weight, 
            witness_merge_fn_t witness_merge_fn,
            witness_t left, 
            witness_t right );

        virtual ~WitnessMerge();

        //! Override Witness::accept
        virtual void accept( Visitor& v, bool visitOnce=false );

        //! Override Witness::reset_marks
        virtual void reset_marks() const;

        //! Override Witness::pretty_print
        virtual std::ostream& prettyPrint( std::ostream& o, size_t depth ) const;

        witness_t left() { return lchild; }

        bool hasLeft() const { return lchild.is_valid(); }

        witness_t right() { return rchild; }

        bool hasRight() const { return rchild.is_valid(); }

      protected:
        witness_merge_fn_t witness_merge_fn; //!< the merge function used
        witness_t lchild; //!< left param to apply
        witness_t rchild; //!< right param to apply

    }; // class WitnessMerge

  } // namespace witness

} // namespace wali

#endif  // wali_witness_WITNESS_MERGE_GUARD


#ifndef wali_witness_WITNESS_MERGE_GUARD
#define wali_witness_WITNESS_MERGE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessMergeFn.hpp"

namespace wali 
{
  namespace witness 
  {
    class WitnessMerge : public Witness 
    {
      public:
        //! Constructor bears witness to a merge function being
        //! applied to left and right with result weight
        WitnessMerge(
            sem_elem_t weight, 
            witness_merge_fn_t witness_merge_fn,
            witness_t caller,
            witness_t rule,
            witness_t callee );

        virtual ~WitnessMerge();

        //! Override Witness::accept
        virtual void accept( Visitor& v, bool visitOnce=false );

        //! Override Witness::reset_marks
        virtual void reset_marks() const;

        //! Override Witness::pretty_print
        virtual std::ostream& prettyPrint( std::ostream& o, size_t depth ) const;

        witness_t caller() { return fCaller; }

        bool hasCaller() const { return fCaller.is_valid(); }

        witness_t rule() { return fRule; }

        bool hasRule() const { return fRule.is_valid(); }

        witness_t callee() { return fCallee; }

        bool hasCallee() const { return fCallee.is_valid(); }

      protected:
        witness_merge_fn_t witness_merge_fn; //!< the merge function used
        witness_t fCaller; //!< caller param to apply
        witness_t fRule;   //!< calling rule
        witness_t fCallee; //!< callee param to apply

    }; // class WitnessMerge

  } // namespace witness

} // namespace wali

#endif  // wali_witness_WITNESS_MERGE_GUARD


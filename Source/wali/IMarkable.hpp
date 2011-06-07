#ifndef wali_IMARKABLE_GUARD
#define wali_IMARKABLE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"

namespace wali
{
  /*!
   * @class IMarkable
   *
   * IMarkable defines the Markable interface. Markable objects are things
   * that need to be placed in a worklist while possibly being in the
   * worklist, and/or objects in cyclic or DAG-like data structures that are
   * traversed in some fashion.
   *
   * IMarkable is implemented in the Markable class. The interface has been
   * extracted out for allowing "delegate" objects to delegate the mark() and
   * unmark() calls to the wrapped object. E.g., the Decorator Pattern.
   *
   * @see Worklist
   */

  class IMarkable
  {
    public:

      /*! Creates a new IMarkable in the unmarked state */
      IMarkable() {}

      /*!
       * This copy constructor actually acts just like the default
       * constructor. This input IMarkable m is ignored. Any time a 
       * IMarkable is created it is "born" in the unmarked state.
       *
       * @param m is ignored
       */
      IMarkable( const IMarkable& m ATTR_UNUSED )
      {
        (void) m;
      }

      /*!
       * IMarkable::operator= has no effect b/c IMarkable has no fields.
       * In general though, the input <b>should be</b> ignored.
       * This is because IMarkable specifies that state may only
       * be changed via the mark and unmark operations.
       */
      IMarkable& operator=( const IMarkable& m ATTR_UNUSED )
      {
        (void) m;
        return *this;
      }

      /*! Destructor does noting */
      virtual ~IMarkable() {}

      /*! Mark this */
      virtual void mark() const throw() = 0;

      /*! Unmark this */
      virtual void unmark() const throw() = 0;

      /*!
       * Check if this is marked.
       *
       * @return true if this is marked
       */
      virtual bool marked() const throw() = 0;

  }; // class IMarkable

} // namespace wali

#endif  // wali_IMARKABLE_GUARD


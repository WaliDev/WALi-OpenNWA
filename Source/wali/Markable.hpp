#ifndef wali_MARKABLE_GUARD
#define wali_MARKABLE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/IMarkable.hpp"

namespace wali
{
  /*!
   * @class Markable
   *
   * Class Markable is a mixin for worklist algorithms.
   * Each instantiation of Markable begins in the unmarked state.
   * A Markable's state may only be changed through mark() and unmark().
   *
   * @see Worklist
   */

  class Markable : public virtual IMarkable
  {
    public:

      /*! Creates a new Markable in the unmarked state */
      Markable();

      /*!
       * This copy constructor actually acts just like the default
       * constructor. This input Markable m is ignored. Any time a 
       * Markable is created it is "born" in the unmarked state.
       *
       * @param m is ignored
       */
      Markable( const Markable& m );

      /*!
       * Markable::operator= has no effect. The input is ignored.
       * This is because Markable specifies that state may only
       * be changed by mark and unmark operations.
       */
      Markable& operator=( const Markable& m );

      virtual ~Markable(); 

      /*! Mark this */
      virtual void mark() const throw();

      /*! Unmark this */
      virtual void unmark() const throw();

      /*!
       * Check if this is marked.
       *
       * @return true if this is marked
       */
      virtual bool marked() const throw();

    private:
      /*! bool marker : true if "this" is marked */
      mutable bool marker;

  }; // class Markable

} // namespace wali

#endif  // wali_MARKABLE_GUARD


#ifndef wali_IMERGE_FN_GUARD
#define wali_IMERGE_FN_GUARD 1

/**
 * @author Akash Lal
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"
#include <iostream>


namespace wali
{
  class IMergeFn;
  typedef ref_ptr< IMergeFn > merge_fn_t;

  /**
   * Class IMergeFn defines the interface
   * that all merge functions must satisfy.
   */
  class IMergeFn : public Printable, public Countable
  {
    public:
      static const std::string XMLTag;

      IMergeFn();

      virtual ~IMergeFn();

      /**
       * Return the result of merging the weight [w1] from the 
       * caller with the weight [w2] that summarizes the callee.
       *
       * @return sem_elem_t summarizing the path from 
       *         call -> enter -> exit -> return
       */
      virtual sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2) = 0;

      /*  
       * Check to see if the two merge functions are equal. This is used to resolve the
       * case when multiple call rules with the rhs are inserted into an EWPDS. Insertion
       * of multiple such rules is only allowed when the merge functions on them are the same.
       * Note that this function can always returns false (in which case an exception will be thrown
       * when multiple call rules with the same rhs are inserted).
       */
      virtual bool equal(merge_fn_t mf) = 0;
      //virtual MergeFn *parse_element(const char *s, sem_elem_t sem) = 0;

  };

} // namespacw wali

#endif // wali_IMERGE_FN_GUARD


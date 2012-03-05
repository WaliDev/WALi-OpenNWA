#ifndef wali_COUNTABLE_GUARD
#define wali_COUNTABLE_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"

namespace wali
{
  class Countable
  {
    public:
      ref_ptr<Countable>::count_t count;

    public:
      /**
       * <b>Note:</b> The capability that a reference counted
       * object should never be reclaimed has been removed.
       * If you require an object to "live forever", then it
       * is up to you to hold onto a reference to the object.
       */
      Countable() : count(0) {}

      /**
       * The copy constructor creates a new instance of
       * Countable, therefore its "count" is initialized to 0.
       */
      Countable( const Countable& c ATTR_UNUSED ) : count(0)
      {
        (void) c;
      }

      /**
       * Countable::operator= does not modify "this's" count.
       * This is because operator= does not modify the number of
       * pointers which refer to this. Therefore, operator= is a 
       * nop.
       */
      Countable& operator=( const Countable& c ATTR_UNUSED ) throw()
      {
        (void) c;
        return *this;
      }

      virtual ~Countable() {}

  }; // class Countable

} // namespace wali

#endif // wali_COUNTABLE_GUARD


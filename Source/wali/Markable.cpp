/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Markable.hpp"

namespace wali
{
  Markable::Markable() : IMarkable(), marker(false) {}

  /*!
   * Anytime a Markable is created it is in the unmarked state.
   * This makes the copy constructor do nothing.
   */
  Markable::Markable( const Markable& m ATTR_UNUSED ) : IMarkable(), marker(false)
  {
    (void) m;
  }

  /*!
   * The Markable object is already created and may or may not
   * be marked. operator= does not perform any action b/c the
   * interface of Markable specifies that state may only
   * be changed through mark and unmark.
   */
  Markable& Markable::operator=( const Markable& m ATTR_UNUSED )
  {
    (void) m;
    return *this;
  }

  Markable::~Markable() {}

  void Markable::mark() const throw()
  {
    marker = true;
  }

  void Markable::unmark() const throw()
  {
    marker = false;
  }

  bool Markable::marked() const throw()
  {
    return marker;
  }
}


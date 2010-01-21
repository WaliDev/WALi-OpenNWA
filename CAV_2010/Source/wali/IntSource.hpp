#ifndef wali_INT_SOURCE_GUARD
#define wali_INT_SOURCE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeySource.hpp"

namespace wali
{
  class IntSource : public KeySource
  {
    public:
      IntSource( int i );

      virtual ~IntSource();

      virtual bool equal( KeySource* rhs );

      virtual size_t hash() const;

      virtual std::ostream& print( std::ostream& o ) const;

      int getInt() const;

    private:
      const int value;

  }; // class IntSource

} // namespace wali

#endif  // wali_INT_SOURCE_GUARD


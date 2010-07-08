#ifndef wali_KEY_SET_SOURCE_GUARD
#define wali_KEY_SET_SOURCE_GUARD 1

/*!
 * @author Amanda Burton
 */

#include "wali/Common.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeyContainer.hpp"

#include <set>

namespace wali
{
  class KeySetSource : public wali::KeySource
  {
    public:
      KeySetSource( std::set<Key> kys );

      virtual ~KeySetSource();

      virtual bool equal( KeySource* rhs );

      virtual size_t hash() const;

      virtual std::ostream& print( std::ostream& o ) const;

      // TODO: probably shouldn't be virtual
      virtual std::set<Key> get_key_set() const;

    protected:
      std::set<Key> kys;

  }; 

} 

#endif  


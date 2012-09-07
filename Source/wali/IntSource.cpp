/**
 * @author Nicholas Kidd
 * @version $Revision: 529 $
 */

#include "wali/Common.hpp"
#include "wali/IntSource.hpp"
#include "wali/hm_hash.hpp"

namespace wali
{
  IntSource::IntSource( int i ) : value(i) {}

  IntSource::~IntSource() {}

  bool IntSource::equal( KeySource* rhs )
  {
    IntSource *isrc = dynamic_cast< IntSource* >(rhs);
    if( isrc != 0 )
      return value == isrc->value;
    else
      return false;
  }

  size_t IntSource::hash() const
  {
    static hm_hash< int > hasher;
    return hasher(value);
  }

  std::ostream& IntSource::print( std::ostream& o ) const
  {
    return o << value;
  }

  int IntSource::getInt() const
  {
    return value;
  }

} // namespace wali


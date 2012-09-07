/**
 * @author Nicholas Kidd
 * @version $Id: StringSource.cpp 529 2009-06-16 02:01:29Z kidd $
 */

#include "wali/StringSource.hpp"
#include "wali/hm_hash.hpp"

namespace wali
{
  StringSource::StringSource( const char* s__ ) :
    s(s__) {}

  StringSource::StringSource( const std::string s__ ) :
    s(s__) {}

  StringSource::~StringSource() {}

  bool StringSource::equal( KeySource* rhs )
  {
    StringSource *ssrc = dynamic_cast< StringSource* >(rhs);
    if( ssrc != 0 )
      return s == ssrc->s;
    else
      return false;
  }

  size_t StringSource::hash() const
  {
    wali::hm_hash< const char * > hasher;
    return hasher( s.c_str() );
  }

  std::ostream& StringSource::print( std::ostream& o ) const
  {
    return o << s;
  }

  std::string StringSource::getString() const
  {
    return s;
  }
}



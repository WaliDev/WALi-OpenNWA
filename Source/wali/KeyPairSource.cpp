/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeyPairSource.hpp"

namespace wali
{
  KeyPairSource::KeyPairSource( Key k1, Key k2 ) : kp(k1,k2) {}

  KeyPairSource::KeyPairSource( const KeyPair& kp_ ) : kp(kp_) {}

  KeyPairSource::~KeyPairSource() {}

  bool KeyPairSource::equal( KeySource* rhs )
  {
    static wali::hm_equal< KeyPair > checker;
    KeyPairSource *kpsrc = dynamic_cast< KeyPairSource* >(rhs);
    if( 0 != kpsrc )
      return checker(kp,kpsrc->kp);
    else
      return false;
  }

  size_t KeyPairSource::hash() const
  {
    static wali::hm_hash< KeyPair > hasher;
    return hasher(kp);
  }

  std::ostream& KeyPairSource::print( std::ostream& o ) const
  {
    o << "(";
    printKey(o,kp.first);
    o << ",";
    printKey(o,kp.second);
    o << ")";
    return o;
  }

  const KeyPair& KeyPairSource::get_key_pair() const
  {
    return kp;
  }

  Key KeyPairSource::first() const
  {
    return kp.first;
  }

  Key KeyPairSource::second() const
  {
    return kp.second;
  }

} // namespace wali


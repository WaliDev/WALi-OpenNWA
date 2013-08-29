/*!
 * @author Nicholas Kidd
 */

#include <memory>
#include "wali/Key.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeySpace.hpp"

namespace wali
{
  static void initEmptyKeySpace(KeySpace * keySpace)
  {
    assert(keySpace->size() == 0);

    // WPDS_EPSILON == 0 == "*"
    Key epsilon = keySpace->getKey("*");
    // NWA_WILD == 1 == "@"
    Key wild = keySpace->getKey("@");

    assert(epsilon == WALI_EPSILON);
    assert(wild == WALI_WILD);
  }

  static KeySpace* createKeySpace()
  {
    KeySpace* keySpace = new KeySpace();
    initEmptyKeySpace(keySpace);
    return keySpace;
  }

  //
  // This class defines the wali::KeySpace
  //
  KeySpace* getKeySpace()
  {
    static std::auto_ptr<KeySpace> keySpace(createKeySpace());
    return keySpace.get();
  }

  void clearKeyspace()
  {
    getKeySpace()->clear();
    initEmptyKeySpace(getKeySpace());
  }

  //
  // Wrapper functions.
  // Call same method of class KeySpace using
  // the wali::KeySpace object.
  //
  Key getKey( key_src_t ks )
  {
    return getKeySpace()->getKey(ks);
  }

  Key getKey( const std::string& s )
  {
      Key ans = getKeySpace()->getKey(s);
      //std::cerr << "[Key " << ans << "]: " << s << "\n";
    return getKeySpace()->getKey(s);
  }

  Key getKey( const char* s )
  {
      Key ans = getKeySpace()->getKey(s);
      //std::cerr << "[Key " << ans << "]: " << s << "\n";
      return ans;
  }

  Key getKey( int i )
  {
      Key ans = getKeySpace()->getKey(i);
      //std::cerr << "[Key " << ans << "]: <" << i << ">\n";
      return ans;
  }

  Key getKey( Key k1, Key k2 )
  {
      Key ans = getKeySpace()->getKey(k1,k2);
      //std::cerr << "[Key " << ans << "]: <" << k1 << "," << k2 << ">\n";
      return ans;
  }

  // @author Amanda Burton
  Key getKey( std::set<Key> ks )
  {
      Key ans = getKeySpace()->getKey(ks);
      //std::cerr << "[Key " << ans << "]: " << key2str(ans) << "\n";
      return ans;
  }


  //
  // Return KeySource associated with the key k
  //
  key_src_t getKeySource( Key k )
  {
    return getKeySpace()->getKeySource(k);
  }

  //
  // Prints key k to std::ostream o
  // Essentially performs the lookup from Key to KeySource and calls
  // KeySource::print
  //
  std::ostream& printKey( std::ostream& o, Key k, bool abbreviate )
  {
    return getKeySpace()->printKey(o,k,abbreviate);
  }

  //
  // Returns string representation of the key
  // Essentially performs the lookup from Key to KeySource and calls
  // KeySource::toString
  //
  std::string key2str( Key k )
  {
    return getKeySpace()->key2str(k);
  }

} // namespace wali


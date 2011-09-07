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
    return getKeySpace()->getKey(s);
  }

  Key getKey( const char* s )
  {
    return getKeySpace()->getKey(s);
  }

  Key getKey( int i )
  {
    return getKeySpace()->getKey(i);
  }

  Key getKey( Key k1, Key k2 )
  {
    return getKeySpace()->getKey(k1,k2);
  }

  // @author Amanda Burton
  Key getKey( std::set<Key> ks )
  {
    return getKeySpace()->getKey(ks);
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


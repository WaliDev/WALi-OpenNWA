/*!
 * @author Nicholas Kidd
 */

#include <cassert>
#include "wali/KeySpace.hpp"
#include "wali/KeySource.hpp"
#include "wali/StringSource.hpp"
#include "wali/IntSource.hpp"
#include "wali/KeyPairSource.hpp"

namespace wali
{
  KeySpace::~KeySpace()
  {
    clear();
  }

  /*!
   * get_key returns the unique wali_key_t associated with the
   * KeySource* ks. If no such key exists, a new wali_key_t will be
   * generated.
   *
   * @see KeySource
   * @see wali_key_t
   *
   * @param KeySource* ks for which a key is sought
   * @return wali_key_t associated with parameter KeySource
   */
  wali_key_t KeySpace::getKey( KeySource* ks )
  {
    ks_hash_map_t::iterator it = keymap.find(ks);
    wali_key_t key;
    if( it != keymap.end() )
    {
      key = it->second;
      // Reclaim the memory. KeySpace assumes ownership of
      // all allocated memory passed to it
      delete ks;
    }
    else {
      key = values.size();
      keymap.insert(ks,key);
      values.push_back(ks);
    }
    return key;
  }

  /*!
   * Wrapper method for createing a StringSource and
   * inserting it into the KeySpace
   */
  wali_key_t KeySpace::getKey( const std::string& s )
  {
    return getKey( new StringSource(s) );
  }

  /*!
   * Wrapper method for createing a StringSource and
   * inserting it into the KeySpace
   */
  wali_key_t KeySpace::getKey( const char* s )
  {
    if( s )
      return getKey( new StringSource(s) );
    else
      return WALI_EPSILON;
  }

  /*!
   * Wrapper method for createing a IntSource and
   * inserting it into the KeySpace
   */
  wali_key_t KeySpace::getKey( int i )
  {
    return getKey( new IntSource(i) );
  }

  /*!
   * Wrapper method for createing a KeyPairSource and
   * inserting it into the KeySpace
   */
  wali_key_t KeySpace::getKey( wali_key_t k1, wali_key_t k2 )
  {
    return getKey( new KeyPairSource(k1,k2) );
  }

  /*!
   * getKeySource retrieves the KeySource* associated to the
   * wali_key_t key. If no such KeySource exists, then a NULL
   * pointer (0) is returned.
   *
   * @see KeySource
   * @see wali_key_t
   *
   * @param key whose correpsonding KeySource* is desired
   * @return KeySource* associated with parameter key
   */
  KeySource* KeySpace::getKeySource( wali_key_t key )
  {
    KeySource* ksrc = 0;
    if( key < size() )
    {
      ksrc = values[key];
    }
    return ksrc;
  }

  /*!
   * Reset the KeySpace. Clears all keys and deletes
   * all KeySources
   */
  void KeySpace::clear()
  {
    keymap.clear();
    ks_vector_t::iterator it = values.begin();
    ks_vector_t::iterator itEND = values.end();
    for( ; it != itEND ; it++ )
    {
      KeySource *ksrc = *it;
      delete ksrc;
      *it = 0;
    }
    values.clear();
    {
      ks_vector_t TEMP;
      TEMP.swap(values);
    }
  }

  /*!
   * Return the number of allocated keys
   */
  size_t KeySpace::size()
  {
    return values.size();
  }

  /*!
   * Helper method that looks up the key and calls KeySource::print
   *
   * @see KeySource
   */
  std::ostream& KeySpace::printKey( std::ostream& o, wali_key_t key )
  {
    KeySource* ksrc = getKeySource(key);
    if( ksrc ) {
      ksrc->print(o);
    }
    else {
      o << "??";
    }
    return o;
  }

  /*!
   * Return std::string rep of KeySource. Looks up the key and calls
   * KeySource::toString(). 
   *
   * @see KeySource
   */
  std::string KeySpace::key2str( wali_key_t key )
  {
    KeySource* ksrc = getKeySource(key);
    if( ksrc ) {
      return ksrc->toString();
    }
    else {
      return "??";
    }
  }

} // namespace wali


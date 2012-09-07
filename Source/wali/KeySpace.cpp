/**
 * @author Nicholas Kidd
 * @version $Id: KeySpace.cpp 1025 2011-05-27 18:47:27Z driscoll $
 */

#include <sstream>
#include <cassert>
#include <cstring>
#include "wali/Common.hpp"
#include "wali/KeySpace.hpp"
#include "wali/KeySource.hpp"
#include "wali/StringSource.hpp"
#include "wali/IntSource.hpp"
#include "wali/KeyPairSource.hpp"
#include "wali/KeySetSource.hpp"

namespace wali
{

  KeySpace::KeySpace()
  {
  }

  KeySpace::~KeySpace()
  {
  }

  /**
   * get_key returns the unique wali_key_t associated with the
   * key_src_t ks. If no such key exists, a new wali_key_t will be
   * generated.
   *
   * @see KeySource
   * @see wali_key_t
   *
   * @param key_src_t ks for which a key is sought
   * @return wali_key_t associated with parameter KeySource
   */
  wali_key_t KeySpace::getKey( key_src_t ks )
  {
    ks_hash_map_t::iterator it = keymap.find(ks);
    wali_key_t key;
    if( it != keymap.end() )
    {
      key = it->second;
    }
    else {
      key = values.size();
      keymap.insert(ks,key);
      values.push_back(ks);
    }
    return key;
  }

  /**
   * Wrapper method for createing a StringSource and
   * inserting it into the KeySpace
   */
  Key KeySpace::getKey( const std::string& s )
  {
    return (s == "") ? WALI_EPSILON : getKey( new StringSource(s) );
  }

  /**
   * Wrapper method for createing a StringSource and
   * inserting it into the KeySpace
   */
  Key KeySpace::getKey( const char* s )
  {
    return ((s == NULL) || (strlen(s) == 0)) ?
      WALI_EPSILON : getKey( new StringSource(s) );
  }

  /**
   * Wrapper method for createing a IntSource and
   * inserting it into the KeySpace
   */
  Key KeySpace::getKey( int i )
  {
    return getKey( new IntSource(i) );
  }

  /**
   * Wrapper method for createing a KeyPairSource and
   * inserting it into the KeySpace
   */
  Key KeySpace::getKey( Key k1, Key k2 )
  {
    return getKey( new KeyPairSource(k1,k2) );
  }

  // @author Amanda Burton  
  wali_key_t KeySpace::getKey( std::set<wali_key_t> kys )
  {
    return getKey( new KeySetSource(kys) );
  }


  /**
   * getKeySource retrieves the key_src_t associated to the
   * Key key. If no such KeySource exists, then a NULL
   * pointer (0) is returned.
   *
   * @see KeySource
   * @see Key
   *
   * @param key whose correpsonding key_src_t is desired
   * @return key_src_t associated with parameter key
   */
  key_src_t KeySpace::getKeySource( Key key )
  {
    key_src_t ksrc = 0;
    if( key < size() )
    {
      ksrc = values[key];
    }
    return ksrc;
  }

  /**
   * Reset the KeySpace. Clears all keys and deletes
   * all KeySources
   */
  void KeySpace::clear()
  {
    keymap.clear();
    {
      ks_vector_t TEMP;
      TEMP.swap(values);
    }
    assert( keymap.size() == 0 );
    assert( values.size() == 0 );
  }

  /**
   * Return the number of allocated keys
   */
  size_t KeySpace::size()
  {
    return values.size();
  }

  /**
   * Helper method that looks up the key and calls KeySource::print
   *
   * @see KeySource
   */
  std::ostream& KeySpace::printKey( std::ostream& o, Key key, bool abbreviate )
  {
    key_src_t ksrc = getKeySource(key);
    if( ksrc.is_valid() ) {
      std::stringstream str;
      ksrc->print(str);

      if(str.str().length() > 20 && abbreviate) {
        o << "[" << key << "]";
      }
      else {
        o << str.str();
      }
    }
    else {
      *waliErr << "[WARNING] Invalid wali::Key(" << key << ")" << std::endl;
      o << "??";
    }
    return o;
  }

  /**
   * Return std::string rep of KeySource. Looks up the key and calls
   * KeySource::toString(). 
   *
   * @see KeySource
   */
  std::string KeySpace::key2str( Key key )
  {
    key_src_t ksrc = getKeySource(key);
    if( ksrc.is_valid() ) {
      return ksrc->toString();
    }
    else {
      return "??";
    }
  }

} // namespace wali


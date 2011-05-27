#ifndef wali_KEY_GUARD
#define wali_KEY_GUARD 1

/**
 * @author Nicholas Kidd
 */

/**
 * @file Key.hpp
 *
 * This file defines the interface to the WALi KeySpace and methods
 * for acting on Keys
 */
#include <string>
#include <set>
#include "wali/ref_ptr.hpp"

namespace wali
{
  /**
   * @typedef Key
   *
   * For now a Key is just an unsigned int
   */
  typedef size_t Key;

  /**
   * Defined in KeySource.hpp
   */
  class KeySource;
  typedef ref_ptr<KeySource> key_src_t;

  /**
   * Defined in KeySpace.hpp
   */
  class KeySpace;

  /**
   * This class defines the wali keyspace
   */
  KeySpace* getKeySpace();

  void clearKeyspace();

  /**
   * Wrapper functions.
   * Call same method of class KeySpace using
   * the wali::KeySpace object.
   */
  Key getKey( key_src_t ks );
  Key getKey( const std::string& s );
  Key getKey( const char* s );
  Key getKey( int i );
  Key getKey( Key k1, Key k2 );
  // @author Amanda Burton
  Key getKey( std::set<Key> ks );

  /**
   * Return KeySource associated with the key k
   */
  key_src_t getKeySource( Key k );

  /*!
   * Prints key k to std::ostream o
   * Essentially performs the lookup from Key to KeySource and calls
   * KeySource::print
   *
   * If abbreviate is true and the string representation of the
   * key being printed is greater than 20 characters, then the
   * actual numeric key is printed instead.
   *
   */
  std::ostream& printKey( std::ostream& o, Key k, bool abbreviate=false );

  /**
   * Returns string representation of the key
   * Essentially performs the lookup from Key to KeySource and calls
   * KeySource::to_string
   */
  std::string key2str( Key k );

} // namespace wali

#endif  // wali_KEY_GUARD


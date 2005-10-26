#ifndef wali_KEY_GUARD
#define wali_KEY_GUARD 1

/*!
 * @author Nick Kidd
 */

/*!
 * @file Key.hpp
 *
 * This file defines the interface to the WALi KeySpace and methods
 * for acting on Keys
 */
#include <string>

namespace wali
{
    /*!
     * @typedef Key
     *
     * For now a Key is just an unsigned int
     */
    typedef size_t Key;

    /*!
     * Defined in KeySource.hpp
     */
    class KeySource;

    /*!
     * Defined in KeySpace.hpp
     */
    class KeySpace;

    /*!
     * This class defines the wali keyspace
     */
    KeySpace* getKeySpace();

    /*!
     * Wrapper functions.
     * Call same method of class KeySpace using
     * the wali::KeySpace object.
     */
    Key getKey( KeySource * ks );
    Key getKey( const std::string& s );
    Key getKey( int i );
    Key getKey( Key k1, Key k2 );

    /*!
     * Return KeySource associated with the key k
     */
    KeySource* getKeySource( Key k );

    /*!
     * Prints key k to std::ostream o
     * Essentially performs the lookup from Key to KeySource and calls
     * KeySource::print
     */
    std::ostream& printKey( std::ostream& o, Key k );

    /*!
     * Returns string representation of the key
     * Essentially performs the lookup from Key to KeySource and calls
     * KeySource::to_string
     */
    std::string key2str( Key k );

} // namespace wali

#endif  // wali_KEY_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

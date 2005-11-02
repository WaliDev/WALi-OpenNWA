/*!
 * @author Nick Kidd
 */

#include "wali/Key.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeySpace.hpp"

namespace wali
{

    //
    // Only used here in Common.cpp
    //
    static KeySpace* createKeySpace()
    {
        KeySpace* keySpace = new KeySpace();
        // WPDS_EPSILON == 0 == "*"
        keySpace->getKey("*");
        return keySpace;
    }

    //
    // This class defines the wali::KeySpace
    //
    KeySpace* getKeySpace()
    {
        static KeySpace *keySpace = createKeySpace();
        return keySpace;
    }

    //
    // Wrapper functions.
    // Call same method of class KeySpace using
    // the wali::KeySpace object.
    //
    Key getKey( KeySource * ks )
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

    //
    // Return KeySource associated with the key k
    //
    KeySource* getKeySource( Key k )
    {
        return getKeySpace()->getKeySource(k);
    }

    //
    // Prints key k to std::ostream o
    // Essentially performs the lookup from Key to KeySource and calls
    // KeySource::print
    //
    std::ostream& printKey( std::ostream& o, Key k )
    {
        return getKeySpace()->printKey(o,k);
    }

    //
    // Returns string representation of the key
    // Essentially performs the lookup from Key to KeySource and calls
    // KeySource::to_string
    //
    std::string key2str( Key k )
    {
        return getKeySpace()->key2str(k);
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

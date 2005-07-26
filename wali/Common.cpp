/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeySpace.hpp"

namespace wali
{

    //
    // This class defines the wali::KeySpace
    //
    KeySpace* getKeySpace()
    {
        static KeySpace *keySpace = new KeySpace();
        return keySpace;
    }

    //
    // Wrapper functions.
    // Call same method of class KeySpace using
    // the wali::KeySpace object.
    //
    wali_key_t getKey( KeySource * ks )
    {
        return getKeySpace()->getKey(ks);
    }

    wali_key_t getKey( const std::string& s )
    {
        return getKeySpace()->getKey(s);
    }

    wali_key_t getKey( int i )
    {
        return getKeySpace()->getKey(i);
    }

    wali_key_t getKey( wali_key_t k1, wali_key_t k2 )
    {
        return getKeySpace()->getKey(k1,k2);
    }

    //
    // Return KeySource associated with the key k
    //
    KeySource* getKeySource( wali_key_t k )
    {
        return getKeySpace()->getKeySource(k);
    }

    //
    // Prints key k to std::ostream o
    // Essentially performs the lookup from wali_key_t to KeySource and calls
    // KeySource::print
    //
    std::ostream& printKey( std::ostream& o, wali_key_t k )
    {
        return getKeySpace()->printKey(o,k);
    }

    //
    // Returns string representation of the key
    // Essentially performs the lookup from wali_key_t to KeySource and calls
    // KeySource::to_string
    //
    std::string key2str( wali_key_t k )
    {
        return getKeySpace()->key2str(k);
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */

/*!
 * @author Nick Kidd
 */

#include <cassert>
#include "wali/KeySpace.hpp"
#include "wali/KeySource.hpp"
#include "wali/StringSource.hpp"
#include "wali/IntSource.hpp"
#include "wali/KeyPairSource.hpp"

namespace wali
{
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
    wali_key_t KeyFactory::get_key( KeySource* ks )
    {
        static wali_key_t INIT_EPS = init_epsilon();
        wali_key_t key = INIT_EPS;
        ks_hash_map_t::iterator it = keymap.find(ks);
        if( it != keymap.end() )
        {
            key = it->second;
            // Reclaim the memory. KeyFactory assumes ownership of
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
     * inserting it into the KeyFactory
     */
    wali_key_t KeyFactory::get_key( const std::string& s )
    {
        return get_key( new StringSource(s) );
    }

    /*!
     * Wrapper method for createing a IntSource and
     * inserting it into the KeyFactory
     */
    wali_key_t KeyFactory::get_key( int i )
    {
        return get_key( new IntSource(i) );
    }

    /*!
     * Wrapper method for createing a KeyPairSource and
     * inserting it into the KeyFactory
     */
    wali_key_t KeyFactory::get_key( wali_key_t k1, wali_key_t k2 )
    {
        return get_key( new KeyPairSource(k1,k2) );
    }

    /*!
     * get_source retrieves the KeySource* associated to the
     * wali_key_t key. If no such KeySource exists, then a NULL
     * pointer (0) is returned.
     *
     * @see KeySource
     * @see wali_key_t
     *
     * @param key whose correpsonding KeySource* is desired
     * @return KeySource* associated with parameter key
     */
    KeySource* KeyFactory::get_source( wali_key_t key )
    {
        KeySource* ksrc = 0;
        if( key < size() )
        {
            ksrc = values[key];
        }
        return ksrc;
    }

    /*!
     * Reset the KeyFactory. Clears all keys and deletes
     * all KeySources
     */
    void KeyFactory::clear()
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
    size_t KeyFactory::size()
    {
        return values.size();
    }

    /*!
     * Helper method that looks up the key and calls KeySource::print
     *
     * @see KeySource
     */
    std::ostream& KeyFactory::print_key( std::ostream& o, wali_key_t key )
    {
        KeySource* ksrc = get_source(key);
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
     * KeySource::to_string(). 
     *
     * @see KeySource
     */
    std::string KeyFactory::key2str( wali_key_t key )
    {
        KeySource* ksrc = get_source(key);
        if( ksrc ) {
            return ksrc->to_string();
        }
        else {
            return "??";
        }
    }

    wali_key_t KeyFactory::init_epsilon()
    {
        assert( values.size() == 0 );
        std::string star("*");
        values.push_back( new StringSource(star) );
        return WALI_EPSILON;
    }

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

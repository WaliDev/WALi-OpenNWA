#ifndef wali_KEY_FACTORY_GUARD
#define wali_KEY_FACTORY_GUARD 1

/*!
 * @author Nick Kidd
 * @version $Revision: 1.5 $
 */

#include "wali/Common.hpp"
#include "wali/HashMap.hpp"
#include "wali/KeySource.hpp"
#include <vector>

namespace wali
{
    /*!
     * @class KeyFactory
     */
    class KeyFactory
    {
        public:
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
            static wali_key_t get_key( KeySource* ks );

            /*!
             * Wrapper method for createing a StringSource and
             * inserting it into the KeyFactory
             */
            static wali_key_t get_key( const std::string& s );

            /*!
             * Wrapper method for createing a IntSource and
             * inserting it into the KeyFactory
             */
            static wali_key_t get_key( int i );

            /*!
             * Wrapper method for createing a KeyPairSource and
             * inserting it into the KeyFactory
             */
            static wali_key_t get_key( wali_key_t k1, wali_key_t k2 );

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
            static KeySource* get_source( wali_key_t key );

            /*!
             * Reset the KeyFactory. Clears all keys and deletes
             * all KeySources
             */
            static void clear();

            /*!
             * Return the number of allocated keys
             */
            static size_t size();

            /*!
             * Helper method that looks up the key and calls KeySource::print
             *
             * @see KeySource
             */
            static std::ostream& print_key( std::ostream& o, wali_key_t key );

            /*!
             * Return std::string rep of KeySource. Looks up the key and calls
             * KeySource::to_string(). 
             *
             * @see KeySource
             */
            static std::string key2str( wali_key_t key );

        protected:
            typedef wali::HashMap< KeySource*,wali_key_t > kf_hash_map_t;
            typedef std::vector< KeySource* > kf_vector_t;

            /*! 
             * keymap maps KeySource* to wali_key_t. The wali_key_t is
             * an index into the vector values
             */
            static kf_hash_map_t keymap;

            /*!
             * wali_key_t's are guaranteed to be unique w.r.t. this KeyFactory
             * because they are indexes into the vector values. KeySource's
             * are retrieved by a lookup into values
             */
            static kf_vector_t values;

        private:

            static wali_key_t init_epsilon();

    }; // class KeyFactory

} // namespace wali

#endif  // wali_KEY_FACTORY_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


#ifndef wali_STRING_SOURCE_GUARD
#define wali_STRING_SOURCE_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeySource.hpp"
#include <string>

namespace wali
{
    /*!
     * @class StringSource
     */
    class StringSource : public KeySource
    {
        public:
            StringSource( const char* s );

            StringSource( const std::string s );

            virtual ~StringSource();

            virtual bool equal( KeySource* rhs );

            virtual size_t hash() const;

            virtual std::ostream& print( std::ostream& o ) const;

        protected:
            std::string s;
    };

} // namespace wali

#endif  // wali_STRING_SOURCE_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


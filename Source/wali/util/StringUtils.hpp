#ifndef STRING_UTILS_GUARD
#define STRING_UTILS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include <string>
#include <vector>

namespace wali
{
    namespace util
    {
        class StringUtils
        {
            public:
                static void Tokenize(
                        const std::string& str,
                        std::vector<std::string>& tokens,
                    const std::string& delimiters );
        };
    }
}

#endif  // STRING_UTILS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

#ifndef wali_util_STRING_UTILS_GUARD
#define wali_util_STRING_UTILS_GUARD 1

/*!
 * @author Nicholas Kidd
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

#endif  // wali_util_STRING_UTILS_GUARD


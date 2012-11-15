#ifndef WALI_UTIL_BASE64_INCULDE
#define WALI_UTIL_BASE64_INCULDE

#include <string>

namespace wali {
    namespace util {
        std::string base64_encode(unsigned char const* , unsigned int len);
        std::string base64_decode(std::string const& s);
    }
}


#endif

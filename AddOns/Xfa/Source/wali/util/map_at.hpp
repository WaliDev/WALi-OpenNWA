#ifndef WALI_UTIL_MAP_GET_INCLUDED
#define WALI_UTIL_MAP_GET_INCLUDED

#include <stdexcept>

namespace wali {
    namespace util {

        /// Given a map 'm' and key 'k', return 'm[k]', except that it works
        /// when 'm' is const and it will throw an exception when 'k' is not
        /// present.
        template<typename Mapping>
        typename Mapping::mapped_type
        map_at(Mapping const & m, typename Mapping::key_type const & k)
        {
            typename Mapping::const_iterator it = m.find(k);
            if (it == m.end()) {
                throw std::out_of_range("Key not present in map");
            }
            return it->second;
        }
            
    }
}

#endif /* WALI_UTIL_MAP_GET_INCLUDED */

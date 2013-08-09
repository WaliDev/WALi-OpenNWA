#ifndef WALI_UTIL_MAP_GET_INCLUDED
#define WALI_UTIL_MAP_GET_INCLUDED

#include <stdexcept>

namespace wali {
    namespace util {

        template<typename Mapping, typename Key, typename Value>
        Value const &
        map_at_impl(Mapping const & m, Key const & k)
        {
            typename Mapping::const_iterator it = m.find(k);
            if (it == m.end()) {
                throw std::out_of_range("Key not present in map");
            }
            return it->second;
        }
        

        /// Given a map 'm' and key 'k', return 'm[k]', except that it works
        /// when 'm' is const and it will throw an exception when 'k' is not
        /// present.
        template<typename Mapping>
        typename Mapping::mapped_type const &
        map_at(Mapping const & m, typename Mapping::key_type const & k)
        {
            return map_at_impl<Mapping,
                               typename Mapping::key_type,
                               typename Mapping::mapped_type>(m, k);
        }
            

        // This is because boost is dumb
        // (https://svn.boost.org/trac/boost/ticket/6031)
        template<typename Mapping>
        typename Mapping::data_type const &
        bimap_at(Mapping const & m, typename Mapping::key_type const & k)
        {
            return map_at_impl<Mapping,
                               typename Mapping::key_type,
                               typename Mapping::data_type>(m, k);
        }

    }
}

#endif /* WALI_UTIL_MAP_GET_INCLUDED */

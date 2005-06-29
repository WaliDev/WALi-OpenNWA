#ifndef wali_KEY_PAIR_GUARD
#define wali_KEY_PAIR_GUARD 1

/*!
 * @author Nick Kidd
 * @version $Revision: 1.3 $
 */

#include "wali/Common.hpp"
#include "wali/hm_hash.hpp"
#include <utility>          //! < provides std::pair

namespace wali
{

    static inline size_t combine_wpds_keys( wali_key_t k1, wali_key_t k2 )
    {
        return k1 + (997*k2);
    }

    /*!
     * KeyPair
     */
    typedef std::pair< wali_key_t,wali_key_t > KeyPair;

    /*!
     * @class Triple
     */
    template< typename T,typename U,typename V > struct Triple
    {
        Triple() {}

        Triple( T t,U u,V v )
            : first(t),second(u),third(v) {}

        Triple( const Triple & rhs )
            : first(rhs.first), second(rhs.second), third(rhs.third) {}

        Triple & operator=( const Triple & rhs )
        {
            first   = rhs.first;
            second  = rhs.second;
            third   = rhs.third;
            return *this;
        }

        bool operator==( const Triple & rhs ) const
        {
            return (
                    (first == rhs.first) &&
                    (second == rhs.second) &&
                    (third == rhs.third)
                   );
        }

        T first;
        U second;
        V third;
    };

    /*!
     * KeyTriple
     */
    typedef Triple< wali_key_t,wali_key_t,wali_key_t > KeyTriple;

    template<> struct hm_hash< KeyPair >
    {
        hm_hash< size_t > hasher;

        size_t operator()( const KeyPair & kp ) const
        {
            return hasher( combine_wpds_keys( kp.first,kp.second ) );
        }

    };

    template<> struct hm_equal< KeyPair >
    {
        bool operator()( const KeyPair& lhs,const KeyPair& rhs ) const
        {
            return ((lhs.first == rhs.first) && (lhs.second == rhs.second));
        }

    };

    template<> struct hm_hash< KeyTriple >
    {

        hm_hash< size_t > hasher;

        size_t operator()( const KeyTriple & kt ) const
        {
            size_t ans = combine_wpds_keys(kt.first,kt.second);
            ans = combine_wpds_keys( ans,kt.third);
            return hasher( ans );
        }

    };

    template<> struct hm_equal< KeyTriple >
    {

        bool operator()( const KeyTriple& lhs,const KeyTriple& rhs ) const
        {
            return lhs == rhs;
        }

    };

}
#endif  // wali_KEY_PAIR_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/


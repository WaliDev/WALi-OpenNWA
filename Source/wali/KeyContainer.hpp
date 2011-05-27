#ifndef wali_KEY_CONTAINER_GUARD
#define wali_KEY_CONTAINER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/hm_hash.hpp"
#include <utility>          // provides std::pair

namespace wali
{
  static inline size_t combineKeys( Key k1, Key k2 )
  {
    return k1 + (997*k2);
  }

  /**
   * KeyPair
   */
  typedef std::pair< Key,Key > KeyPair;

  /**
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

    // @author Amanda Burton
	  bool operator<( const Triple & rhs ) const
	  {
		  if( first == rhs.first )
		  {
			  if( second == rhs.second )
				  return (third < rhs.third);
			  else
				  return (second < rhs.second);
		  }
		  else
			  return (first < rhs.first);
	  }

    T first;
    U second;
    V third;
  };

  /**
   * KeyTriple
   */
  typedef Triple< Key,Key,Key > KeyTriple;

  /**
   * Return a Triple with the given types (analgous to std::make_pair)
   */
  template< typename T,typename U,typename V >
  Triple<T, U, V>
  make_triple(T const & t, U const & u, V const & v)
  {
    return Triple<T, U, V>(t, u, v);
  }

  // @author Amanda Burton
  /**
   * @class Quad
   */
  template< typename T,typename U,typename V,typename W > struct Quad
  {
    Quad() {}

    Quad( T t,U u,V v,W w )
      : first(t),second(u),third(v),fourth(w) {}

    Quad( const Quad & other )
      : first(other.first), second(other.second), third(other.third),
      fourth(other.fourth) {}

    Quad & operator=( const Quad & other )
    {
      first   = other.first;
      second  = other.second;
      third   = other.third;
      fourth  = other.fourth;
      return *this;
    }

    bool operator==( const Quad & other ) const
    {
      return ((first == other.first) &&
        (second == other.second) &&
        (third == other.third) &&
        (fourth == other.fourth)); 
    }

    bool operator<( const Quad & other ) const
    {
      if( first == other.first )
      {
        if( second == other.second )
        {
          if( third == other.third )
            return (fourth < other.fourth);
          else
            return (third < other.third);
        }
        else
          return (second < other.second);
      }
      else
        return (first < other.first );
    }

    T first;
    U second;
    V third;
    W fourth;
  };

  /**
   * KeyQuad
   */
  typedef Quad< Key,Key,Key,Key > KeyQuad;

  /**
   * Return a Quad with the given types (analgous to std::make_pair)
   */
  template< typename T,typename U,typename V,typename W >
  Quad<T, U, V, W>
  make_quad(T const & t, U const & u, V const & v, W const & w)
  {
    return Quad<T, U, V, W>(t, u, v, w);
  }


  template<> struct hm_hash< KeyPair >
  {
    hm_hash< size_t > hasher;

    size_t operator()( const KeyPair & kp ) const
    {
      return hasher( combineKeys( kp.first,kp.second ) );
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
      size_t ans = combineKeys(kt.first,kt.second);
      ans = combineKeys( ans,kt.third);
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
  
  // @author Amanda Burton
  template<> struct hm_hash< std::set<Key> >
  {
    hm_hash< size_t > hasher;

    size_t operator()( const std::set<Key> & ks ) const
    {
      size_t key = 0;
      for( std::set<Key>::const_iterator it = ks.begin();
            it != ks.end(); it++ )
      {
        key = hasher( combineKeys( key,*it ) );
      } 
      return key;      
    }

  };

  template<> struct hm_equal< std::set<Key> >
  {
    bool operator()( const std::set<Key>& lhs,const std::set<Key>& rhs ) const
    {
      if( lhs.size() != rhs.size() )
        return false;
      for( std::set<Key>::const_iterator lit = lhs.begin(), rit = rhs.begin();
            (lit != lhs.end()) && (rit != rhs.end()); lit++,rit++ )
      {
        if( *lit != *rit )
          return false;
      }
      return true;
    }

  };

} // namespace wali

#endif  // wali_KEY_CONTAINER_GUARD


#ifndef wali_HM_HASH_GUARD
#define wali_HM_HASH_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include <cstring> // for hm_equal< char * >


/* These were taken from Thomas Wang
 * http://www.concentric.net/~Ttwang/tech/inthash.htm
 *
 * On machines where long is 64 bits, the below should
 * probably be used.
 *
 */

#undef longhash1
#define longhash1(key)  \
{                       \
  key += ~(key << 32);  \
  key ^= (key >> 22);  \
  key += ~(key << 13);  \
  key ^= (key >> 8);   \
  key += (key << 3);    \
  key ^= (key >> 15);  \
  key += ~(key << 27);  \
  key ^= (key >> 31);  \
}


#undef jenkins_mix
#define jenkins_mix(a,b,c) \
{ \
  a=a-b;  a=a-c;  a=a^(c>>13); \
  b=b-c;  b=b-a;  b=b^(a<<8);  \
  c=c-a;  c=c-b;  c=c^(b>>13); \
  a=a-b;  a=a-c;  a=a^(c>>12); \
  b=b-c;  b=b-a;  b=b^(a<<16); \
  c=c-a;  c=c-b;  c=c^(b>>5);  \
  a=a-b;  a=a-c;  a=a^(c>>3);  \
  b=b-c;  b=b-a;  b=b^(a<<10); \
  c=c-a;  c=c-b;  c=c^(b>>15); \
}

#undef jenkins_wrapper
#define jenkins_wrapper( c )            \
{                                       \
  size_t a = 0x9e3779b9;     \
  size_t b = 0x9e3779b9;     \
  jenkins_mix(a,b,c);             \
}

#undef primitive_type_hash
#define primitive_type_hash( key ) \
{                                       \
  key += (key << 12);                 \
  key ^= (key >> 22);                 \
  key += (key << 4);                  \
  key ^= (key >> 9);                  \
  key += (key << 10);                 \
  key ^= (key >> 2);                  \
  key += (key << 7);                  \
  key ^= (key >> 12);                 \
}

namespace wali 
{

  /*
   * hm_hash is a templated functor to provide defaults
   * for hashing the c++ primitive types.  It is defined
   * because std::hash<> is currently an SGI extension
   * to the STL.
   */

  template< typename T > struct hm_hash { };
  template< typename T > struct hm_equal {};

  /*
   * Primitives hm_hash is defined for (includes const prim):
   *      int
   *      unsigned int
   *      char
   *      unsigned char
   *      char *
   */


  /********************/
  /* char             */
  /********************/
  template<> struct hm_hash< char >
  {
    size_t operator()( char key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_hash< const char >
  {
    size_t operator()( const char key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_equal< char >
  {
    bool operator()( char lhs,char rhs ) const
    {
      return lhs == rhs;
    }
  };
  template<> struct hm_equal< const char >
  {
    bool operator()( const char lhs,const char rhs ) const
    {
      return lhs == rhs;
    }
  };

  /********************/
  /* unsigned char    */
  /********************/
  template<> struct hm_hash< unsigned char >
  {
    size_t operator()( unsigned char key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_hash< const unsigned char >
  {
    size_t operator()( const unsigned char key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_equal< unsigned char >
  {
    bool operator()( unsigned char lhs,unsigned char rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const unsigned char >
  {
    bool operator()( const unsigned char lhs,const unsigned char rhs ) const
    {
      return lhs == rhs;
    }
  };

  /********************/
  /* int              */
  /********************/
  template<> struct hm_hash< int >
  {
    size_t operator()( int key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_hash< const int >
  {
    size_t operator()( const int key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_equal< int >
  {
    bool operator()( int lhs,int rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const int >
  {
    bool operator()( const int lhs,const int rhs ) const
    {
      return lhs == rhs;
    }
  };

  /********************/
  /* unsigned int */
  /********************/
  template<> struct hm_hash< unsigned int >
  {
    size_t operator()( int key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_hash< const unsigned int >
  {
    size_t operator()( const unsigned int key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_equal< unsigned int >
  {
    bool operator()( unsigned int lhs,unsigned int rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const unsigned int >
  {
    bool operator()( const unsigned int lhs,const unsigned int rhs ) const
    {
      return lhs == rhs;
    }
  };

  /********************/
  /* long              */
  /********************/
  template<> struct hm_hash< long >
  {
    size_t operator()( long key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_hash< const long >
  {
    size_t operator()( const long key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_equal< long >
  {
    bool operator()( long lhs,long rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const long >
  {
    bool operator()( const long lhs,const long rhs ) const
    {
      return lhs == rhs;
    }
  };

  /********************/
  /* unsigned long    */
  /********************/
  template<> struct hm_hash< unsigned long >
  {
    size_t operator()( unsigned long key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_hash< const unsigned long >
  {
    size_t operator()( const unsigned long key ) const
    {
      size_t sz = key;
      primitive_type_hash( sz );
      return sz;
    }
  };

  template<> struct hm_equal< unsigned long >
  {
    bool operator()( unsigned long lhs,unsigned long rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const unsigned long >
  {
    bool operator()( const unsigned long lhs,const unsigned long rhs ) const
    {
      return lhs == rhs;
    }
  };

  /********************/
  /* char*            */
  /********************/
  /* The hash function used was found at:
   *      http://www.cs.yorku.ca/~oz/hash.html
   *
   * The algorithm (djb2) is attributed to Dan Bernstein
   * on the web page.  It claims to have been posted on the
   * newsgroup comp.lang.c so I am assuming it is in the public
   * domain.
   */
  static unsigned long strhashfn( const char* str )
  {
    unsigned long hash = 5381;
    int c;

    while (0 != (c = *str++))
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
  }

  template<> struct hm_hash< char * >
  {
    size_t operator()( char * str ) const
    {
      return strhashfn( str );
    }
  };

  template<> struct hm_hash< const char * >
  {
    size_t operator()( const char * str ) const
    {
      return strhashfn( str );
    }
  };

  template<> struct hm_equal< char * >
  {
    bool operator()( char * lhs,char * rhs ) const
    {
      return (0 == strcmp(lhs,rhs));
    }
  };

  template<> struct hm_equal< const char * >
  {
    bool operator()( const char * lhs,const char * rhs ) const
    {
      return (0 == strcmp(lhs,rhs));
    }
  };

  /********************/
  /* long long        */
  /********************/
  template<> struct hm_hash< long long >
  {
    size_t operator()( long long key ) const
    {
      longhash1( key );
      return (size_t)key;
    }
  };

  template<> struct hm_hash< const long long >
  {
    size_t operator()( const long long key ) const
    {
      long long sz = key;
      longhash1( sz );
      return (size_t)sz;
    }
  };

  template<> struct hm_equal< long long >
  {
    bool operator()( long long lhs , long long rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const long long >
  {
    bool operator()( const long long lhs , const long long rhs ) const
    {
      return lhs == rhs;
    }
  };

  /**********************/
  /* unsigned long long */
  /**********************/
  template<> struct hm_hash< unsigned long long >
  {
    size_t operator()( unsigned long long key ) const
    {
      longhash1( key );
      return (size_t)key;
    }
  };

  template<> struct hm_hash< const unsigned long long >
  {
    size_t operator()( const unsigned long long key ) const
    {
      unsigned long long sz = key;
      longhash1( sz );
      return (size_t)sz;
    }
  };

  template<> struct hm_equal< unsigned long long >
  {
    bool operator()( unsigned long long lhs , unsigned long long rhs ) const
    {
      return lhs == rhs;
    }
  };

  template<> struct hm_equal< const unsigned long long >
  {
    bool operator()( const unsigned long long lhs , const unsigned long long rhs ) const
    {
      return lhs == rhs;
    }
  };

} // namespace wali

#endif  // wali_HM_HASH_GUARD


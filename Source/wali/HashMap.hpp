#ifndef wali_HASH_MAP_GUARD
#define wali_HASH_MAP_GUARD 1

/**
 * @author Nicholas Kidd
 */

// Disable name truncation in Visual Studio
#if defined(_WIN32) && _MSC_VER > 1000
#   pragma warning(disable: 4786)
#endif

#include <climits> // ULONG_MAX
#include <utility>  // std::pair
#include <functional>
#include <iostream>
#include "wali/hm_hash.hpp"
#define HASHMAP_GROWTH_FRACTION 0.75
#define HASHMAP_SHRINK_FRACTION 0.25


/*
 * TODO??:  make erase shrink the number of buckets
 * TODO??:  make GROWTH|HASHMAP_SHRINK_FRACTION member vars vs. static
 */

namespace wali
{

#if defined(PI_STATS_DETAIL) && PI_STATS_DETAIL
  // totalHashMapnumBuckets of all HashMaps
  extern long totalHashMapnumBuckets;
#endif /* PI_STATS_DETAIL */

  // Pre decls
  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc > class HashMap;

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc > class HashMapIterator;

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc > class HashMapConstIterator;

  // FIXME
  template< typename Value > struct Bucket
  {
    Bucket( const Value& v, Bucket *n=0 )
      : value(v),next(n) {}
    Value value;
    Bucket *next;
  };

  /**
   * One should always use:
   *      HashMap< a,b,c,d >::iterator
   *              or
   *      HashMap< a,b,c,d >::const_iterator
   *
   * This keeps with abstraction.
   */
  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      class HashMapIterator
      {
        public:
          friend class HashMap< Key,Data,HashFunc,EqualFunc >;
          friend class HashMapConstIterator< Key,Data,HashFunc,EqualFunc >;

        public:
          typedef HashMapIterator< Key,Data,HashFunc,EqualFunc >      iterator;
          typedef HashMapConstIterator< Key,Data,HashFunc,EqualFunc > const_iterator;
          typedef HashMap< Key,Data,HashFunc,EqualFunc >              hashmap_type;
          typedef std::pair< Key,Data >                               pair_type;
          typedef pair_type                                           value_type;
          typedef size_t                                              size_type;
          typedef Bucket< value_type >                                bucket_type;


          HashMapIterator() : bucket(0),hashMap(0) {}

          HashMapIterator( bucket_type *bkt,hashmap_type *hmap )
            : bucket( bkt ),hashMap(hmap) {}

          inline value_type *operator->()
          {
            return &(bucket->value);
          }

          inline value_type& operator*()
          {
            return bucket->value;
          }

          inline bool operator==( const iterator& right )
          {
            return right.bucket == bucket;
          }

          inline bool operator!=( const iterator& right )
          {
            return right.bucket != bucket;
          }

          inline iterator operator++()
          {
            return this->operator++(60);
          }

          HashMapIterator operator++( int )
          {
            bucket_type *old = bucket;
            bucket = bucket->next;
            if( !bucket ) {
              size_type bktNum = hashMap->bucketFromValue( old->value );
              while( !bucket && ++bktNum < hashMap->numBuckets ) {
                bucket = hashMap->buckets[bktNum];
              }
            }
            return *this;
          }

        protected:
          bucket_type  *bucket;
          hashmap_type *hashMap;

      };

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      class HashMapConstIterator
      {
        friend class HashMap< Key,Data,HashFunc,EqualFunc >;
        friend class HashMapIterator< Key,Data,HashFunc,EqualFunc >;

        public:
          typedef HashMapIterator< Key,Data,HashFunc,EqualFunc >      iterator;
          typedef HashMapConstIterator< Key,Data,HashFunc,EqualFunc > const_iterator;
          typedef HashMap< Key,Data,HashFunc,EqualFunc >              hashmap_type;
          typedef std::pair< Key,Data >                               pair_type;
          typedef pair_type                                           value_type;
          typedef size_t                                              size_type;
          typedef Bucket< value_type >                                bucket_type;

          HashMapConstIterator() : bucket(0),hashMap(0) {}

          HashMapConstIterator( const bucket_type *bkt,const hashmap_type *hmap )
            : bucket( bkt ),hashMap(hmap) {}

          HashMapConstIterator( const iterator& it )
            : bucket( it.bucket ),hashMap( it.hashMap ) {}

          inline const value_type *operator->()
          {
            return &(bucket->value);
          }

          inline const value_type& operator*()
          {
            return bucket->value;
          }

          inline bool operator==( const const_iterator& right )
          {
            return right.bucket == bucket;
          }

          inline bool operator!=( const const_iterator& right )
          {
            return right.bucket != bucket;
          }

          const_iterator operator++()
          {
            return this->operator++(60);
          }

          HashMapConstIterator operator++( int )
          {
            const bucket_type *old = bucket;
            bucket = bucket->next;
            if( !bucket ) {
              size_type bktNum = hashMap->bucketFromValue( old->value );
              while( !bucket && ++bktNum < hashMap->numBuckets ) {
                bucket = hashMap->buckets[bktNum];
              }
            }
            return *this;
          }

        protected:
          const bucket_type  *bucket;

          const hashmap_type *hashMap;
      };


  /**
   * class HashMap
   *
   * Notes:
   *      equal_to is part of the STL.
   */
  template< typename Key,
    typename Data,
    typename HashFunc = hm_hash< Key >,
    typename EqualFunc = hm_equal< Key > >
      class HashMap 
      {

        public:     // typedef
          typedef HashMapIterator< Key,Data,HashFunc,EqualFunc >      iterator;
          typedef HashMapConstIterator< Key,Data,HashFunc,EqualFunc > const_iterator;
          typedef HashMap< Key,Data,HashFunc,EqualFunc >              hashmap_type;
          typedef std::pair< Key,Data >                               pair_type;
          typedef pair_type                                           value_type;
          typedef size_t                                              size_type;
          typedef Bucket< value_type >                                bucket_type;

          typedef Key   key_type;
          typedef Data  mapped_type;

          // friend iterator
          friend class HashMapIterator<Key,Data,HashFunc,EqualFunc>;
          friend class HashMapConstIterator<Key,Data,HashFunc,EqualFunc>;

          enum enum_size_type_max { SIZE_TYPE_MAX = ULONG_MAX };

        public:     // con/destructor
          HashMap( size_type the_size=47 )
            : numValues(0),numBuckets(the_size), 
              growthFactor( static_cast<double>(the_size) * HASHMAP_GROWTH_FRACTION ), 
              shrinkFactor( static_cast<double>(the_size) * HASHMAP_SHRINK_FRACTION )
        { initBuckets(); }

          HashMap( const HashMap& hm )
          {
            operator=(hm);
          }

          HashMap& operator=( const HashMap& hm ) {
            clear();
            for( const_iterator it = hm.begin() ; it != hm.end() ; it++ ) {
              insert(key(it),value(it));
            }
            return *this;
          }

          ~HashMap() {
            clear();
            releaseBuckets();
          }

        public:        // inline methods
          void clear()
          {
            for(size_type i=0;i<numBuckets;i++) {
              while( buckets[i] ) {
                bucket_type *tmp = buckets[i];
                buckets[i] = tmp->next;
                releaseBucket( tmp );
              }
            }
            numValues = 0;
          }

          inline size_type size() const
          {
            return numValues;
          }

          inline size_type capacity() const
          {
            return numBuckets;
          }

          inline std::pair<iterator,bool> insert( const Key& k, const Data& d )
          {
            return insert( pair_type(k,d) );
          }

          void erase( const Key& key_to_erase )
          {
            iterator it = find( key_to_erase );
            if( it != end() )
              erase( it );
          }

          iterator begin()
          {
            for( size_type n = 0 ; n < numBuckets ; n++ )
              if( buckets[n] )
                return iterator(buckets[n],this);
            return end();
          }

          inline iterator end()
          {
            return iterator( 0,this );
          }

          const_iterator begin() const
          {
            for( size_type n = 0 ; n < numBuckets ; n++ )
              if( buckets[n] )
                return const_iterator(buckets[n],this);
            return end();
          }

          inline const_iterator end() const
          {
            return const_iterator(0,this);
          }

          Key & key( iterator & it )
          {
            return it->first;
          }

          const Key & key( const_iterator & it ) const
          {
            return it->first;
          }

          Data & value( iterator & it )
          {
            return it->second;
          }

          const Data & value( const_iterator & it ) const
          {
            return it->second;
          }

          Data & data( iterator & it )
          {
            return it->second;
          }

          const Data & data( const_iterator & it ) const
          {
            return it->second;
          }

          void print_stats( std::ostream & o = std::cout ) const 
          {
            size_type i= 0;
            int activebuckets= 0;
            int bucket_count=0;
            int max_bucket_count=0;
            int min_bucket_count = 10000000;
            bucket_type *bkt;
            for( ; i< numBuckets; i++ ) {
              bkt = buckets[i];
              if( bkt ) {
                activebuckets++;
                bucket_count = 0;
                while( bkt ) {
                  bucket_count++;
                  bkt = bkt->next;
                }
                if( bucket_count > max_bucket_count )
                  max_bucket_count = bucket_count;
                if( bucket_count < min_bucket_count )
                  min_bucket_count = bucket_count;
              }
            }
            o << "Stats:\n";
            o << "\tNumber of Values   : " << numValues << std::endl;
            o << "\tNumber of Buckets  : " << numBuckets << std::endl;
            o << "\tActive buckets     : " << activebuckets << std::endl;
            o << "\tAverage bucket size: " << (numValues / activebuckets)  << std::endl;
            o << "\tMax bucket count   : " << max_bucket_count << std::endl;
            o << "\tMin bucket count   : " << min_bucket_count << std::endl;
          }

        public:        // methods
          std::pair<iterator,bool> insert( const value_type& );
          iterator find( const Key& );
          const_iterator find( const Key& ) const;
          void erase( iterator it );
          Data & operator[](const Key & k) {
            return (*((insert(value_type(k, Data()))).first)).second;
          }

        public:        // vars

        private:    // inline methods
          /*************  Get Bucket location from various objects **********/
          inline size_type _bucketFromHash( size_type hash, size_type the_size ) const
          {
            return hash % the_size;
          }
          inline size_type _bucketFromKey( const Key& the_key,size_type the_size ) const
          {
            return _bucketFromHash( hashFunc(the_key),the_size );
          }
          inline size_type _bucketFromValue( const value_type& v,size_type s ) const
          {
            return _bucketFromKey( v.first,s );
          }
          inline size_type bucketFromHash( size_type hash ) const
          {
            return _bucketFromHash( hash,numBuckets );
          }
          inline size_type bucketFromKey( const Key& the_key ) const
          {
            return bucketFromHash( hashFunc(the_key) );
          }
          inline size_type bucketFromValue( const value_type& the_value ) const
          {
            return bucketFromKey( the_value.first );
          }

          /************ Initialize and release buckets *************************/
          inline void initBuckets()
          {
            buckets = _makeBuckets( numBuckets );
          }

          inline bucket_type **  _makeBuckets( size_type the_size )
          {
#if defined(PI_STATS_DETAIL) && PI_STATS_DETAIL
            totalHashMapnumBuckets += the_size;
#endif /* PI_STATS_DETAIL */
            bucket_type **bktptr = new bucket_type*[the_size];
            memset(bktptr,0,sizeof(bucket_type*)*the_size);
            //for(the_size_type s = 0;s<the_size;s++)
            //bktptr[s] = 0;
            return bktptr;
          }

          inline void releaseBuckets()
          {
#if defined(PI_STATS_DETAIL) && PI_STATS_DETAIL
            totalHashMapnumBuckets -= numBuckets;
#endif /* PI_STATS_DETAIL */
            delete[] buckets;
          }

          inline void releaseBucket( bucket_type *bkt )
          {
            delete bkt;
          }

        private:    // methods
          void resize( size_type the_size );

        private:    // variables
          bucket_type **buckets;
          size_type numValues;
          size_type numBuckets;
          double growthFactor;
          double shrinkFactor;
          HashFunc hashFunc;
          EqualFunc equalFunc;
      };

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      HashMapIterator< Key,Data,HashFunc,EqualFunc >
      HashMap< Key,Data,HashFunc,EqualFunc>::find( const Key& the_key )
      {
        size_type bktNum = bucketFromKey( the_key );
        for( bucket_type *bkt = buckets[bktNum]; bkt; bkt = bkt->next )
          if( equalFunc( the_key,bkt->value.first) )
            return iterator( bkt,this );
        return end();
      }

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      HashMapConstIterator< Key,Data,HashFunc,EqualFunc >
      HashMap< Key,Data,HashFunc,EqualFunc>::find( const Key& the_key ) const
      {
        size_type bktNum = bucketFromKey( the_key );
        for( bucket_type *bkt = buckets[bktNum]; bkt; bkt = bkt->next )
          if( equalFunc( the_key,bkt->value.first) )
            return const_iterator( bkt,this );
        return end();
      }

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      void HashMap<Key,Data,HashFunc,EqualFunc>::erase(
          typename HashMap<Key,Data,HashFunc,EqualFunc>::iterator it )
      {
        /* We can't just erase the bucket in the iterator b/c we
         * need to fix the "pointers" in the bucket list.
         *
         * ie   prev -> it.bkt -> next  ==> prev -> next
         *
         * This would be simple if we were using doubly linked list..
         */
        bucket_type *bkt = it.bucket;
        if( bkt ) {
          // get the first bucket in chain
          size_type bktNum = bucketFromValue( it.bucket->value );
          bucket_type *cur = buckets[bktNum];
          if( cur == bkt ) {
            buckets[bktNum] = cur->next;
            releaseBucket( cur );
            numValues--;
          }
          else {
            bucket_type *next = cur->next;
            while( next ) {
              if( next == bkt ) {
                cur->next = next->next;
                releaseBucket( next );
                numValues--;
                break;
              }
              else {
                cur = next;
                next = cur->next;
              }
            }
          }
        }
      }

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      std::pair< HashMapIterator< Key,Data,HashFunc,EqualFunc >,bool >
      HashMap<Key,Data,HashFunc,EqualFunc>::insert( const value_type& the_value )
      {
        typedef std::pair< iterator,bool > RPair;
        resize( numValues+1 );
        size_type bktNum = bucketFromValue( the_value );
        bucket_type *bktptr = buckets[bktNum];
        for( bucket_type *tmp = bktptr; tmp ; tmp = tmp->next ) {
          if( equalFunc( the_value.first,tmp->value.first ) ) {
            return RPair( iterator(tmp,this),false );
          }
        }
        bktptr = new bucket_type( the_value,bktptr );
        buckets[bktNum] = bktptr;
        numValues++;
        return RPair( iterator(bktptr,this),true );
      }

  template< typename Key,
    typename Data,
    typename HashFunc,
    typename EqualFunc >
      void HashMap<Key,Data,HashFunc,EqualFunc>::resize( size_type needed )
      {
        if( needed < growthFactor )
          return;
        size_type new_size = numBuckets * 2;
        if( new_size >= SIZE_TYPE_MAX )
          return;

#ifdef DBGHASHMAP
        printf("DBG HashMap : Resizing to %lu buckets\n",new_size);
#endif

        // alloc new array of buckets
        bucket_type **tmp = _makeBuckets( new_size );

        // copy from old array to new
        size_type newBktNum;
        for(size_type i=0;i<numBuckets;i++) {
          /*
          // is this ok?
          if( buckets[i] ) {
          bucket_type *bktptr = buckets[i];
          newBktNum = _bucketFromValue( bktptr->value,new_size );
          tmp[newBktNum] = bktptr;
          buckets[i] = 0;
          }
          */
          while( buckets[i] ) {
            bucket_type *old = buckets[i];
            // get new bucket num from hashing
            newBktNum = _bucketFromValue( old->value,new_size );
            // store old's next ptr
            buckets[i] = old->next;
            // set up old's new bktNum
            old->next = tmp[newBktNum];
            // put buck in new array
            tmp[newBktNum] = old;
          }
        }

        // release old array
        releaseBuckets();

        // set buckets to new array
        buckets = tmp;
        numBuckets = new_size;
        // set up new growth|shrink factors
        growthFactor = static_cast<double>(numBuckets) * HASHMAP_GROWTH_FRACTION;
        shrinkFactor = static_cast<double>(numBuckets) * HASHMAP_SHRINK_FRACTION;
      }

} // namespace wali

#endif  // wali_HASH_MAP_GUARD


#ifndef wali_REF_PTR_GUARD
#define wali_REF_PTR_GUARD 1

/**
 * @author Nicholas Kidd
 * @author Evan Driscoll
 */

#include <cstdio>
#include <cassert>
#include <climits>
#include <iostream>

namespace wali
{

  /**
   * @class ref_ptr
   * @brief A reference counting pointer class
   * @warning This class is *NOT* thread safe.
   *
   * The templated class should use the mixin Countable. When using Countable
   * simply pass a boolean true or false to the rcmix constructor.  The default
   * value is true and this means the object will be reference counted.  A
   * passed in value of false means to not reference count the object (ie. it
   * will never be deleted ).
   *
   * If you prefer not to inherit from Countable, then the templated class 
   * must have a member variable named count that can be accessed from 
   * ref_ptr and modified by ref_ptr. the count variable should have
   * operator++(),operator--(), and operator==( int ) defined.  As a 
   * note, this class was designed with count being an unsigned integer.
   *
   * Count should be initialized to 0 for proper reference
   * couting to work.  If it is desirable for the pointer/object
   * to never be deleted, initialize count >= 1.  WARNING: don't forget
   * to initialize count to 0 in the copy constructor!  (And don't
   * forget to have a copy constructor.)
   *
   * @see Countable
   *
   */

  template< typename T > class ref_ptr
  {

    public:
      typedef unsigned int count_t;

      ref_ptr( T *t = 0 ) {
        acquire(t);
      }

      ref_ptr( const ref_ptr& rp ) {
        acquire( rp.ptr );
      }

      /**
       * @brief This will succeed if S is a subclass of T.
       */
      template< typename S > ref_ptr<T>( const ref_ptr<S>& rp ) {
        acquire( rp.get_ptr() );
      }

      ~ref_ptr() {
        release();
      }

      ref_ptr& operator=( T * t ) {
        if( ptr != t ) {
          T * old_ptr = ptr;
          acquire(t);
          release(old_ptr);
        }
        return *this;
      }

      ref_ptr& operator=( const ref_ptr& rp ) {
        return *this = rp.get_ptr();
      }

      bool operator==(const ref_ptr& that) const {
        return ptr == that.get_ptr();
      }

      bool operator!=(const ref_ptr& that) const {
        return ptr != that.get_ptr();
      }

      T * get_ptr() const {
        return ptr;
      }

      T * operator->() const {
        return ptr;
      }

      T& operator*()   const {
        assert(0 != ptr);
        return *ptr;
      }

      bool operator<(ref_ptr const & that) const {
        return ptr < that.get_ptr();
      }

      bool is_empty() const { return (0 == ptr); }
      bool is_valid() const { return !(is_empty());}

    private:
      T * ptr;

      void acquire( T * t )
      {
        ptr = t;
        if( t ) {
          ++t->count;
#ifdef DBGREFPTR
          std::cout << "Acquired " << t << " with count = "
            << t->count << std::endl;
#endif
        }
      }

      static void release( T * old_ptr )
      {
        if( old_ptr ) {
          --old_ptr->count;
#ifdef DBGREFPTR
          std::cout << "Released " << *old_ptr << " with count = "
            << old_ptr->count << std::endl;
#endif
          if( old_ptr->count == 0 ) {
#ifdef DBGREFPTR
            std::cout << "Deleting ptr: " << *old_ptr << std::endl;
#endif
            delete old_ptr;
          }
        }
      }

      void release() {
        release(ptr);
        ptr = 0;
      }

  }; // class ref_ptr

} // namespace wali

#endif  // wali_REF_PTR_GUARD


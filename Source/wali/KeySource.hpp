#ifndef wali_KEY_SOURCE_GUARD
#define wali_KEY_SOURCE_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Countable.hpp"
#include "wali/Printable.hpp"
#include "wali/hm_hash.hpp"

namespace wali
{
  /**
   * @class KeySource
   */
  class KeySource : public Printable, public Countable
  {
  public:
    KeySource() {}
    
    virtual ~KeySource() {}
    
    virtual bool equal( KeySource* rhs ) = 0;
    
    virtual size_t hash() const = 0;
    
    virtual std::ostream& print( std::ostream& o ) const = 0;
    
  protected:
    
  }; // class KeySource

  template<> struct hm_hash< key_src_t >
  {
    size_t operator()( key_src_t ksrc ) const
    {
      return ksrc->hash();
    }
  };

  template<> struct hm_equal< key_src_t >
  {
    bool operator()( key_src_t lhs, key_src_t rhs ) const
    {
      return lhs->equal(rhs.get_ptr());
    }
  };

}; // namespace wali

#endif // wali_KEY_SOURCE_GUARD


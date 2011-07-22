/*!
 * @author Amanda Burton
 */

#include "KeySetSource.hpp"

namespace wali
{
  KeySetSource::KeySetSource( std::set<Key> key_set ) 
  {
    kys = key_set;
  }

  KeySetSource::~KeySetSource() {}

  bool KeySetSource::equal( KeySource* rhs )
  {
    static wali::hm_equal< std::set<Key> > checker;
    KeySetSource *kssrc = dynamic_cast< KeySetSource* >(rhs);
    if( 0 != kssrc )
      return checker(kys,kssrc->kys);
    else
      return false;
      
  }

  size_t KeySetSource::hash() const
  {
    static wali::hm_hash< std::set<Key> > hasher;
    return hasher(kys);
  }

  std::ostream& KeySetSource::print( std::ostream& o ) const
  {
    o << "{";
    bool first = true;
    for( std::set<Key>::const_iterator it = kys.begin();
          it != kys.end(); it++,first = false )
    {
      if(!first)
        o << ",";
      printKey(o,*it);
    }        
    o << "}";
    return o;
  }

  std::set<Key> KeySetSource::get_key_set() const
  {
    return kys;
  }

} 


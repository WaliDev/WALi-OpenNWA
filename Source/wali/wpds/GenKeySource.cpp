/*!
 * @author Nicholas Kidd
 * $Id: GenKeySource.cpp 418 2008-05-12 17:19:03Z kidd $
 */

#include "wali/Common.hpp"
#include "wali/wpds/GenKeySource.hpp"
#include "wali/hm_hash.hpp"

namespace wali
{
  namespace wpds {
    GenKeySource::GenKeySource( size_t g, Key k) : gen(g),key(k) 
    {
      //            std::cout << "GenKeySource : ";
      //            print(std::cout);
      //            std::cout << std::endl;
    }

    GenKeySource::~GenKeySource() {}

    bool GenKeySource::equal( KeySource* rhs )
    {
      GenKeySource *that = dynamic_cast< GenKeySource* >(rhs);
      if( that != 0 )
        return (gen == that->gen) && (key == that->key);
      else
        return false;
    }

    size_t GenKeySource::hash() const
    {
      hm_hash< size_t > hasher;
      size_t v = gen + (997*key);
      return hasher(v);
    }

    std::ostream& GenKeySource::print( std::ostream& o ) const
    {
      printKey(o,key) << "_g" << gen;
      return o;
    }

    size_t GenKeySource::getGeneration() const {
      return gen;
    }

    Key GenKeySource::getKey() const {
      return key;
    }

  } // namespace wpds

} // namespace wali


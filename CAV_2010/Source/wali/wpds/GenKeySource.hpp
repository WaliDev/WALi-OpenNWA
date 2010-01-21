#ifndef wali_wpds_GENKEY_SOURCE_GUARD
#define wali_wpds_GENKEY_SOURCE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/KeySource.hpp"

namespace wali
{
  namespace wpds {
    class GenKeySource : public KeySource
    {
      public:
        GenKeySource( size_t g, Key k );

        virtual ~GenKeySource();

        virtual bool equal( KeySource* rhs );

        virtual size_t hash() const;

        virtual std::ostream& print( std::ostream& o ) const;

        size_t getGeneration() const;

        Key getKey() const;

      protected:
        size_t gen;
        Key key;

    }; // class GenKeySource

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_GENKEY_SOURCE_GUARD


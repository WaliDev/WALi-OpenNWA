#ifndef wali_WEIGHT_FACTORY_GUARD
#define wali_WEIGHT_FACTORY_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include <string>

namespace wali
{
  class WeightFactory
  {
    public:
      WeightFactory();

      virtual ~WeightFactory();

      virtual wali::sem_elem_t getWeight( std::string s ) = 0;

  }; // class WeightFactory

} // namespace wali

#endif  // wali_WEIGHT_FACTORY_GUARD


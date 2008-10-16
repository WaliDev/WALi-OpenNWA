#ifndef wali_WEIGHT_HANDLER_GUARD
#define wali_WEIGHT_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"

#include "wali/SemElem.hpp"

#include "wali/IWaliHandler.hpp"

namespace wali
{

  class IWeightHandler : public IWaliHandler
  {

    public:
      IWeightHandler();

      virtual ~IWeightHandler();

      /**
       * @return true if (str == wali::SemElem::XMLTag)
       */
      virtual bool handlesElement( std::string str );

      /**
       * @return the sem_elem_t for the parsed weight.
       */
      virtual sem_elem_t getWeight() = 0;

  }; // class IWeightHandler


} // namespace wali

#endif  // wali_WEIGHT_HANDLER_GUARD



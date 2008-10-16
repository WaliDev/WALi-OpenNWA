#ifndef wali_WEIGHT_FACTORY_HANDLER_GUARD
#define wali_WEIGHT_FACTORY_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/IWeightHandler.hpp"

namespace wali
{
  class WeightFactoryHandler;

  /**
   * Class for backwards compatability with WeightFactories.
   */
  class WeightFactoryHandler : public IWeightHandler
  {
    public:
      WeightFactoryHandler(WeightFactory& wf);

      virtual ~WeightFactoryHandler();

      /**
       * @return the weight that WeightFactory 
       * parses out of the character string [str]
       * from : "<Weight>str</Weight>"
       */
      virtual sem_elem_t getWeight();

      /**
       * Override to catch the endElement and
       * invoke
       *
       *   WeightFactory.getWeight( getCharactersString() )
       */
      virtual void endElement( const XMLCh* const uri,
          const XMLCh* const localname,
          const XMLCh* const qname);

    protected:
      WeightFactory& fWeightFactory;
      sem_elem_t se; /** Hold the weight returned by WeightFactory for </Weight> */
  };

} // namespace wali

#endif  // wali_WEIGHT_FACTORY_HANDLER_GUARD



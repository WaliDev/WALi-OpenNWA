#ifndef wali_WEIGHT_FACTORY_HANDLER_GUARD
#define wali_WEIGHT_FACTORY_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/IUserHandler.hpp"

namespace wali
{
  class WeightFactory;
  class MergeFnFactory;

  /**
   * Class for backwards compatability with WeightFactories.
   */
  class UserFactoryHandler : public IUserHandler
  {
    public:
      UserFactoryHandler(WeightFactory* wf,MergeFnFactory* mf);

      virtual ~UserFactoryHandler();

      /**
       * @return the weight that WeightFactory 
       * parses out of the character string [str]
       * from : "<Weight>str</Weight>"
       */
      virtual sem_elem_t getWeight();

      /**
       * @return the merge function that MergeFnFactory
       * parses out of the character string [str]
       * from : "<MergeFn>str</MergeFn>
       */
      virtual merge_fn_t getMergeFn();

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
      WeightFactory* fWeightFactory;
      MergeFnFactory* fMergeFactory;
      sem_elem_t fWeight; /** Hold the weight returned by WeightFactory for </Weight> */
      merge_fn_t fMergeFn; /** Hold the parsed merge_fn_t */
  };

} // namespace wali

#endif  // wali_WEIGHT_FACTORY_HANDLER_GUARD



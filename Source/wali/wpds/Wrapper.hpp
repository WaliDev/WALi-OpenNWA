#ifndef wali_wpds_WRAPPER_GUARD
#define wali_wpds_WRAPPER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Countable.hpp"
#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"

namespace wali
{
  namespace wfa {
    class ITrans;
  }

  namespace wpds
  {
    class Rule;

    namespace ewpds {
      class ERule;
    }

    /**
     * @class Wrapper
     * 
     * This class defines the interface used to "wrap"
     * weights on rules and transitions. A Wrapper can be passed
     * to a WPDS when it is created for adding functionality, e.g.
     * Witnesses. The base class Wrapper is simply the identity wrapper.
     * It returns the weight on the Rule and Trans when wrap is invoked and
     * the same weight when unwrap is invoked.
     *
     * Wrappers can be chained together. It is up to the Wrapper definer
     * (i.e., the user) to take care of proper chaining. WALi assumes that
     * unwrap will return an element from the user defined weight domain.
     */
    class Wrapper : public Countable
    {
      public:
        Wrapper() : Countable() {}

        virtual ~Wrapper() {}

        virtual sem_elem_t wrap( wfa::ITrans const & t );

        virtual sem_elem_t wrap( wpds::Rule const & r );

        /**
         * Wrapping of a rule's weight happens before wrapping
         * of its merge function. Thus, the weight
         *
         *    r->weight()
         *    
         * has already been wrapped. For example, see how
         * this is handled in wali::witness::WitnessWrapper.
         *
         * @see wali::witness::WitnessWrapper
         */
        virtual merge_fn_t wrap( wpds::ewpds::ERule const & r, merge_fn_t user_merge);

        virtual sem_elem_t unwrap( sem_elem_t se );

        virtual merge_fn_t unwrap( merge_fn_t mf );

      protected:

    }; // class Wrapper

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_WRAPPER_GUARD


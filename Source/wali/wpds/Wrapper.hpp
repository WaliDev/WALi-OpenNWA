#ifndef wali_WRAPPER_GUARD
#define wali_WRAPPER_GUARD 1

/*!
 * @author Nick Kidd
 */
#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/wpds/ewpds/MergeFunction.hpp"

namespace wali
{
    namespace wfa {
        class Trans;
    }

    namespace wpds
    {
        class Rule;

        /*!
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
        class Wrapper
        {
            public:
                typedef wpds::ewpds::merge_fn_t merge_fn_t;

            public:
                Wrapper() {}

                virtual ~Wrapper() {}

                virtual sem_elem_t wrap( wfa::Trans& t );

                virtual sem_elem_t wrap( wpds::Rule& r );

                virtual merge_fn_t wrap( merge_fn_t );

                virtual sem_elem_t unwrap( sem_elem_t se );

            protected:

        }; // class Wrapper

    } // namespace wpds

} // namespace wali

#endif  // wali_WRAPPER_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */

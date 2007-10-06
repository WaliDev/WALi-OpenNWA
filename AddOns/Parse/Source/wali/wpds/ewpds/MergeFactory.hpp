#ifndef wali_MERGE_FACTORY_GUARD
#define wali_MERGE_FACTORY_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/ewpds/MergeFunction.hpp"
#include <string>

namespace wali
{
    namespace wpds
    {
        namespace ewpds
        {
            class MergeFactory
            {
                public:
                    MergeFactory();

                    virtual ~MergeFactory();

                    virtual wali::wpds::ewpds::merge_fn_t getMergeFn( std::string& s ) = 0;

            }; // class MergeFactory
        }
    }
} // namespace wali

#endif  // wali_MERGE_FACTORY_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/


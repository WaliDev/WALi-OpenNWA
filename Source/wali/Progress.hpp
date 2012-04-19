#ifndef wali_PROGRESS_GUARD
#define wali_PROGRESS_GUARD 1

#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"

/**
 * @author David Magagnosc
 */

namespace wali
{
    class Progress;
    typedef ref_ptr<Progress> progress_t;

    /*! @class Progress
     *
     * Progress provides a mechanism for wali clients to receive
     * progress notification by deriving their own subclass.  The
     * tick() function will be called periodically during wali
     * processing.
     */

    class Progress : public Countable
    {
    public:
        virtual ~Progress() {};
        virtual void tick() = 0;
    };

} // namespace wali

#endif  // wali_PROGRESS_GUARD

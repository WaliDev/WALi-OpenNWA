#ifndef wali_util_TIME_GUARD
#define wali_util_TIME_GUARD

#include <iosfwd>
#include <string>
#include <sys/time.h>
#include <iostream>

/*
 * @author Ben Liblit (NAK - I think)
 *
 * This only works on Linux (maybe other OSes that
 * support /proc filesystem)
 */
namespace wali {

    namespace util {

        class Timer {
            public:
                static bool measureAndReport;

                Timer(const std::string &task, std::ostream& os=std::cerr);
                double elapsed() const;
                ~Timer();

            private:
                const timeval start;
                const std::string task;
                std::ostream& os; //!< for reporting

                static timeval now();
        };

    } // namespace util

} // namespace wali


#endif // wali_util_TIME_GUARD

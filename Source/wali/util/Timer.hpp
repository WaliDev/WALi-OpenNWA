#ifndef wali_util_TIME_GUARD
#define wali_util_TIME_GUARD

#include <iosfwd>
#include <string>
#include <sys/time.h>
#include <iostream>
//#include <cunistd>
#include <cstdlib>
#include <cstdio>
#include "wali/Printable.hpp"

/*
 * @author Ben Liblit (NAK - I think)
 *
 * This only works on Linux (maybe other OSes that
 * support /proc filesystem)
 */
namespace wali {

    namespace util {

        struct mytime {
            clock_t t;
        };

        class Timer : public wali::Printable {
            public:
                static bool measureAndReport;

                Timer(const std::string &task, std::ostream& os=std::cerr);

                ~Timer();

                double elapsed() const;

                virtual std::ostream& print( std::ostream& out ) const;

                static std::ostream& printTime( std::ostream& out, clock_t clk );

                static mytime now();

            private:
                const mytime start;
                const std::string task;
                std::ostream& os; //!< for reporting

        };

    } // namespace util

} // namespace wali


#endif // wali_util_TIME_GUARD

#ifndef wali_util_TIME_GUARD
#define wali_util_TIME_GUARD

#include <iosfwd>
#include <string>
#include <sys/times.h>
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

        class Timer : public wali::Printable {
            public:
                static bool measureAndReport;
                static const long TIMER_CLK_TICK;

                Timer(const std::string &task, std::ostream& os=*waliErr);

                ~Timer();

                double elapsed() const;

                virtual std::ostream& print( std::ostream& out ) const;

                static std::ostream& printTime( std::ostream& out, clock_t clk );

            private:
                const clock_t start;
                struct tms st_tms;
                const std::string task;
                std::ostream& os; //!< for reporting

        };

    } // namespace util

} // namespace wali


#endif // wali_util_TIME_GUARD

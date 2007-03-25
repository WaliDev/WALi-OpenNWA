#include <iomanip>
#include <iostream>
#include <sys/times.h>
#include <unistd.h>

#include "wali/Common.hpp"

#include "wali/util/Timer.hpp"

clock_t sum_tms( const struct tms& t ) {
#if 0
    *wali::waliErr << "\tDebug Timer:\n";
    *wali::waliErr << "\t\ttms_utime : " << t.tms_utime << "\n";
    *wali::waliErr << "\t\ttms_stime : " << t.tms_stime << "\n";
    *wali::waliErr << "\t\ttms_cutime : " << t.tms_cutime << "\n";
    *wali::waliErr << "\t\ttms_cstime : " << t.tms_cstime << "\n";
#endif
    clock_t c = t.tms_utime 
        + t.tms_stime + t.tms_cutime + t.tms_cstime;
    return c;
}

namespace wali {

    namespace util {

        bool Timer::measureAndReport = true;
        const long Timer::TIMER_CLK_TICK = sysconf(_SC_CLK_TCK);


        Timer::Timer(const std::string &task, std::ostream& os)
            : start(clock()), task(task), os(os)
        {
            times(&st_tms);
        }

        Timer::~Timer()
        {
            if (measureAndReport)
                print( os );
        }

        double Timer::elapsed() const
        {

            struct tms end_tms;
            clock_t now = times(&end_tms);
            if( now == -1 ) {
                *waliErr << "[WARNING] times() returned an error.\n";
                return 0;
            }
            else {
                clock_t t = sum_tms(end_tms) - sum_tms(st_tms);
                return (t * 1.0) / (double)(TIMER_CLK_TICK);
            }

            //const mytime end = now();
            //clock_t t = (end.t - start.t);
            //return t * 1.0 / (double)(CLOCKS_PER_SEC);
        }


        std::ostream& Timer::print( std::ostream& out ) const
        {
            const double difference = elapsed();
            out << task << ": " << difference << " secs\n";
            return out;
        }

        std::ostream& Timer::printTime(std::ostream& out, clock_t clk ) {
            return out << (clk * 1.0 / (double)(CLOCKS_PER_SEC));
        }

    } // namespace util

} // namespace wali

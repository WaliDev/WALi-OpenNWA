#include <iomanip>
#include <iostream>
#include "wali/util/Timer.hpp"

namespace wali {

    namespace util {

        bool Timer::measureAndReport = true;


        Timer::Timer(const std::string &task, std::ostream& os)
            : start(now()), task(task), os(os)
        {
        }

        double Timer::elapsed() const
        {
            if (measureAndReport)
            {
                const timeval end = now();
                return
                    (end.tv_sec  - start.tv_sec) +
                    (end.tv_usec - start.tv_usec) / 1e6;
            }
            else
                return 0;
        }

        Timer::~Timer()
        {
            if (measureAndReport)
            {
                const double difference = elapsed();
                os << task << ": " << std::setprecision(2) << difference << " secs\n";
            }
        }


        timeval Timer::now()
        {
            timeval result;

            if (measureAndReport)
                gettimeofday(&result, 0);
            else
                result.tv_sec = result.tv_usec = 0;

            return result;
        }

    } // namespace util

} // namespace wali

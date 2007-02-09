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

        Timer::~Timer()
        {
            if (measureAndReport)
                print( os );
        }

        double Timer::elapsed() const
        {
            const mytime end = now();
            clock_t t = (end.t - start.t);
            return t * 1.0 / (double)(CLOCKS_PER_SEC);
        }

        std::ostream& Timer::print( std::ostream& out ) const
        {
            const double difference = elapsed();
            os << task << ": " << difference << " secs\n";
        }

        std::ostream& Timer::printTime(std::ostream& out, clock_t clk ) {
            return out << (clk * 1.0 / (double)(CLOCKS_PER_SEC));
        }

        mytime Timer::now()
        {
            mytime result;
            result.t = clock();
            return result;
        }

    } // namespace util

} // namespace wali

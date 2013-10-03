#ifndef wali_util_TIME_GUARD
#define wali_util_TIME_GUARD

#include <iosfwd>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#ifndef _WIN32
#  include <sys/times.h>
#else
#  include <windows.h>
#  include <time.h>
#endif

/*
 * @author Ben Liblit (NAK - I think)
 *
 * This only works on Linux (maybe other OSes that
 * support /proc filesystem)
 */
namespace wali {

  namespace util {


    namespace details {
      static const long long NANO = 1000000000;

      inline
      long long
      now() {
#if defined(CLOCK_MONOTONIC)
        timespec ts;
        int err = clock_gettime(CLOCK_MONOTONIC, &ts);
        assert(err == 0);
        return (static_cast<long long>(ts.tv_sec) * NANO) + ts.tv_nsec;
#elif defined(_WIN32)
        LARGE_INTEGER now;
        BOOL err = QueryPerformanceCounter(&now);
        assert(err);
        return now.QuadPart;
#else
        return 0;
#endif
      }

      inline
      double
      to_sec(long long ticks) {
#ifndef _WIN32
        return static_cast<double>(ticks)/NANO;
#else
        LARGE_INTEGER now;
        BOOL err = QueryPerformanceFrequency(&now);
        assert(err);
        return static_cast<double>(ticks)/now.QuadPart;
#endif
      }
    }


    class GoodTimer {
    public:
      typedef long long int64;
      
      int64 _current_start;
      int64 _total_time;
      int _depth;
      const char * _name;
    public:
      GoodTimer(const char * c)
        : _current_start(0)
        , _total_time(0)
        , _depth(0)
        , _name(c)
      {
        start();
      }

      ~GoodTimer() {
        stop();
        std::cout << "Timer: " << _name << ": " << total_time() << "\n";
      }
      
      void start() {
        assert((_current_start == 0) ^ (_depth > 0));
        ++_depth;
        if (_depth == 1) {
          _current_start = details::now();
        }
      }

      void stop() {
        assert((_depth > 0) && (_current_start != 0));
        --_depth;
        if (_depth == 0) {
          int64 end = details::now();
          _total_time += (end - _current_start);
          _current_start = 0;
        }
      }

      double total_time() const {
        return details::to_sec(_total_time);
      }
    };


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
#ifndef _WIN32
        struct tms st_tms;
#endif
        const std::string task;
        std::ostream& os; //!< for reporting

    };

  } // namespace util

} // namespace wali


#endif // wali_util_TIME_GUARD


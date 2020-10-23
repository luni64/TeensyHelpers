#pragma once

#include "Arduino.h"
#include <chrono>

#include "cycles64.h"

struct teensy_clock
{
    // required typdefs:
    using duration = std::chrono::duration<uint64_t, std::ratio<1, F_CPU>>;  // use a uint64_t representation with a time step of 1/F_CPU (=1.667ns @600MHz)
    using rep = duration::rep;                                               // uint64_t
    using period = duration::period;                                         // std::ratio<1,600>
    using time_point = std::chrono::time_point<teensy_clock, duration>;

    static constexpr bool is_steady = false;                                 // can not be guaranteed to be steady (could be readjusted by syncToRTC)

    static time_point now()
    {
        duration t = duration(t0 + cycles64::get());                         // adds the current 64bit cycle counter to an offset set by syncToRTC() (default: t0=0)
        return time_point(t);                                                // ... and returns the correspoinging time point.
    }

    static void begin(bool sync = true);                                     // starts the 64bit cycle counter update interrupt. Sync=true sycns the clock to the RTC
    static void syncToRTC();                                                 // Sync to RTC whenever needed (e.g. after adjusting the RTC)

    //Map to C API
    static std::time_t to_time_t(const time_point& t);                       // returns the time_t value (seconds since 1.1.1970) to be used with standard C-API functions
    static time_point from_time_t(std::time_t t);                            // converts a time_t value to a time_point

 private:
    static uint64_t t0;                                                      // offset to adjust time (seconds from 1.1.1970 to now).
};
    
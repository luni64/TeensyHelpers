#include "teensy_clock.h"
#include "cycles64.h"

void teensy_clock::begin(bool sync)
{
    cycles64::begin();
    if (sync) syncToRTC();
}

std::time_t teensy_clock::to_time_t(const time_point& t)
{
    using namespace std::chrono;
    return std::time_t(duration_cast<seconds>(t.time_since_epoch()).count());
}

teensy_clock::time_point teensy_clock::from_time_t(std::time_t t)
{
    using namespace std::chrono;
    typedef std::chrono::time_point<teensy_clock, seconds> from;
    return time_point_cast<duration>(from(seconds(t)));
}

void teensy_clock::syncToRTC()
{
    t0 = ((uint64_t)rtc_get()) * F_CPU - cycles64::get();
}

uint64_t teensy_clock::t0 = 0;

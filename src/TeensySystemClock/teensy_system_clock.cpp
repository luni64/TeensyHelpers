#include <chrono>
#include "imxrt.h"

using namespace std::chrono;

system_clock::time_point system_clock::now()
{
    //20.4.1.3 Reading RTC and SRTC values (nr. of 32768kHz periods)
    uint32_t hi1 = SNVS_HPRTCMR;
    uint32_t lo1 = SNVS_HPRTCLR;
    while (1)
    {
        uint32_t hi2 = SNVS_HPRTCMR;
        uint32_t lo2 = SNVS_HPRTCLR;
        if (lo1 == lo2 && hi1 == hi2) break;
        hi1 = hi2;
        lo1 = lo2;
    }

    //convert to nanoseconds
    uint64_t t = (((uint64_t)hi1 << 32) | lo1) * (1.0E9 / 32768.0);

    return time_point(duration(t));
}
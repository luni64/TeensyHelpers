#pragma once

#include "Arduino.h"
#include <functional>

class IntervalTimerEx : public IntervalTimer
{
 public:
    template <typename period_t>  // using a template to avoid replicating the varios begin functions of the IntervalTimer
    bool begin(std::function<void()> callback, period_t period)
    {
        for (index = 0; index < 4; index++)                          // find the next free slot
        {
            if (callbacks[index] == nullptr)                         // ->free
            {
                if (IntervalTimer::begin(relays[index], period))     // we got a slot but we need to also get an actual timer
                {
                    callbacks[index] = callback;                     // if ok -> store callback
                    return true;
                }
                return false;
            }
        }
        return false; // can never happen if bookkeeping is ok
    }

    void end()
    {
        callbacks[index] = nullptr;
        IntervalTimer::end();
    }

    ~IntervalTimerEx() { end(); }

 protected:
    unsigned index = 0;

    static std::function<void()> callbacks[4];

    using relay_t = void (*)();
    static relay_t relays[4];
};
#include "IntervalTimerEx.h"

using relay_t = void (*)();
using callback_t = std::function<void(void)>;

// generate and preset the callback storage
callback_t IntervalTimerEx::callbacks[4]{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};


// generate the static array of relay functions
relay_t IntervalTimerEx::relays[4]{
    [] { callbacks[0](); },
    [] { callbacks[1](); },
    [] { callbacks[2](); },
    [] { callbacks[3](); },
};
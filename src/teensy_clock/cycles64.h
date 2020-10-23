#pragma once
/************************************************************************************
 * Implements a 64bit extension of the cycle counter (ARM_DWT_CYCCNT). The extension
 * only works if get() is called at least once per overflow period (~7s @600MHz).
 * Thus, the code sets up the periodic interrupt of the SNVS module (Real Time Clock)
 * to call get once per second automaticall.
 *
 * begin():  sets up the periodic RTC interrupt to update the counter
 * get():    returns the 64bit cycle counter
 *
 * luni64 10-2020, Licence: MIT
 ************************************************************************************/

#include <cstdint>

namespace cycles64
{
    extern void begin();
    extern uint64_t get();
}
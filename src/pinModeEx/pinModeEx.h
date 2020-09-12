#pragma once

#include <initializer_list>
#include "core_pins.h"

inline void pinMode(std::initializer_list<uint8_t> pins, uint8_t mode)
{
    for (uint8_t pin : pins)
    {
        pinMode(pin, mode);
    }
}

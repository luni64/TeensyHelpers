#include "MicroModT4.h"

namespace MMT
{
    void BUS::pinMode(int mode) const
    {
        for (uint8_t pin : {G0, G1, G2, G3, G4, G5, G6, G7})
        {
            ::pinMode(pin, mode);
        }
    }

    void BUS::operator=(uint8_t value) const
    {
        noInterrupts();      
        GPIO7_DR           = (GPIO7_DR & ~(0xFF << 4)) | (((uint32_t)value) << 4);
        interrupts();
    }

    BUS::operator uint8_t() const
    {
        return (uint8_t) (GPIO7_PSR >> 4);
    }

    void pinMode(const BUS& bus, int mode)
    {
        bus.pinMode(mode);
    }

    BUS& mmBus = BUS::getInstance();
}
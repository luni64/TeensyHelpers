#pragma once
#include "Arduino.h"

namespace MMT
{
    //GPIO ---------------------------------------
    constexpr uint8_t G0  = 40; // GPIO7-04
    constexpr uint8_t G1  = 41; // GPIO7-05
    constexpr uint8_t G2  = 42; // GPIO7-06
    constexpr uint8_t G3  = 43; // GPIO7-07
    constexpr uint8_t G4  = 44; // GPIO7-08
    constexpr uint8_t G5  = 45; // GPIO7-09
    constexpr uint8_t G6  = 6;  // GPIO7-10
    constexpr uint8_t G7  = 9;  // GPIO7-11
    constexpr uint8_t G8  = 26; // GPIO6-30
    constexpr uint8_t G9  = 32; // GPIO7-12
    constexpr uint8_t G10 = 33; // GPIO9-07
    constexpr uint8_t G11 = 27; // GPIO6-31

    class BUS
    {
     public:
        void operator=(uint8_t value) const;
        operator uint8_t() const;

     protected:
        void pinMode(int mode) const;
        friend void pinMode(const BUS&, int);
    };
    //void pinMode(const MMT::BUS&, int mode);

    // ADC ---------------------------------------
    constexpr uint8_t A0 = ::A0;
    constexpr uint8_t A1 = ::A1;

    // DAC / PWM ---------------------------------
    constexpr uint8_t PWM0 = 3;
    constexpr uint8_t PWM1 = 2;

    // UARTs -------------------------------------
    constexpr uint8_t TX1 = 1; // Serial 1
    constexpr uint8_t RX1 = 0;
    //constexpr uint8_t RTS1 = 25; // not in schematic
    //constexpr uint8_t CTS1 = 32; // not in schematic
    constexpr uint8_t TX2 = 17; // Serial 2
    constexpr uint8_t RX2 = 16;

    // I2C ---------------------------------------
    constexpr uint8_t SCL0   = 19;
    constexpr uint8_t SDA0   = 18;
    constexpr uint8_t I2CINT = 29;
    constexpr uint8_t SCL1   = 24;
    constexpr uint8_t SDA1   = 25;

    // SPI --------------------------------------
    constexpr uint8_t SCK  = 13;
    constexpr uint8_t CIPO = 12;
    constexpr uint8_t MISO = 12;
    constexpr uint8_t COPI = 11;
    constexpr uint8_t MOSI = 11;
    constexpr uint8_t CS   = 10;

    // CAN --------------------------------------
    constexpr uint8_t CAN_TX = 31;
    constexpr uint8_t CAN_RX = 30;

}

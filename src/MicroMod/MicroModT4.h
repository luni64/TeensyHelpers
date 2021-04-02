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

    constexpr uint8_t D0  = 4;  // GPIO9-06
    constexpr uint8_t D1  = 5;  // GPIO9-08

    // ADC ---------------------------------------
    constexpr uint8_t A0  = ::A0;
    constexpr uint8_t A1  = ::A1;
    constexpr uint8_t A2  = ::A2;
    constexpr uint8_t A3  = ::A3;
    constexpr uint8_t A8  = ::A8;
    constexpr uint8_t A9  = 23;
    constexpr uint8_t A10 = 24;
    constexpr uint8_t A11 = 25;
    constexpr uint8_t A12 = G8;
    constexpr uint8_t A13 = G11;

    // DAC / PWM ---------------------------------
    constexpr uint8_t PWM0 = 3;
    constexpr uint8_t PWM1 = 2;

    // UARTs -------------------------------------
    constexpr uint8_t TX1 = 1; // Serial 1
    constexpr uint8_t RX1 = 0;
    //constexpr uint8_t RTS1 = ?; // not in schematic
    //constexpr uint8_t CTS1 = ?; // not in schematic
    constexpr uint8_t TX2 = 17; // Serial 2
    constexpr uint8_t RX2 = 16;

    // I2C ---------------------------------------
    constexpr uint8_t SCL    = A5;
    constexpr uint8_t SDA    = A4;
    constexpr uint8_t I2CINT = 29;
    constexpr uint8_t SCL1   = A10;
    constexpr uint8_t SDA1   = A11;

    // SPI --------------------------------------
    constexpr uint8_t SCK  = 13;
    constexpr uint8_t CIPO = 12;
    constexpr uint8_t MISO = 12;
    constexpr uint8_t COPI = 11;
    constexpr uint8_t MOSI = 11;
    constexpr uint8_t CS   = 10;

    // SDI --------------------------------------
    constexpr uint8_t SDIO_CLK   = 36;
    constexpr uint8_t SDIO_CMD   = 37;
    constexpr uint8_t SDIO_DATA0 = 35;
    constexpr uint8_t SDIO_DATA1 = 34;
    constexpr uint8_t SDIO_DATA2 = 38;
    constexpr uint8_t SDIO_DATA3 = 39;

    // CAN --------------------------------------
    constexpr uint8_t CAN_TX = 31;
    constexpr uint8_t CAN_RX = 30;

    // CAMERA -----------------------------------
    constexpr uint8_t CAM_TRIG  = D1;
    constexpr uint8_t CAM_HSYNC = G9;
    constexpr uint8_t CAM_VSYNC = G10;
    constexpr uint8_t CAM_MCLK  = 7;
    constexpr uint8_t CAM_PCLK  = 8;

    // AUDIO -----------------------------------
    constexpr uint8_t AUD_MCLK  = A9;
    constexpr uint8_t AUD_OUT   = CAM_MCLK;
    constexpr uint8_t AUD_IN    = CAM_PCLK;
    constexpr uint8_t AUD_LRCLK = A6;
    constexpr uint8_t AUD_BCLK  = A7;

    // Misc
    constexpr uint8_t BATT_VIN = A8;

    class BUS
    {
     public:
        void operator=(uint8_t value) const;
        operator uint8_t() const;
        void pinMode(int mode) const;

        static BUS& getInstance()
        {
            static BUS instance;
            return instance;
        }

     private:
        BUS() {}
    };

    extern BUS& mmBus;
}

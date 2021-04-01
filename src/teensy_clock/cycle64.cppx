

#include "Arduino.h"

namespace cycles64
{
    uint64_t get();

    namespace // private -----------------------------
    {
        uint32_t oldLow = ARM_DWT_CYCCNT;
        uint32_t curHigh = 0;

        void SNVS_isr(void)
        {
            SNVS_HPSR |= 0b11; // reset interrupt flag
            get();             // call to check for overflow
            asm("dsb");        // prevent double calls of the isr
        }

    } // end private namespace <<---------------------

    uint64_t get()
    {
        noInterrupts();
        uint32_t curLow = ARM_DWT_CYCCNT;
        if (curLow < oldLow) // roll over
        {
            curHigh++;
        }
        oldLow = curLow;
        uint64_t curVal = ((uint64_t)curHigh << 32) | curLow;
        interrupts();

        return curVal;
        
    }

    void begin()
    {
        // disable periodic snvs interrupt
        SNVS_HPCR &= ~SNVS_HPCR_PI_EN;
        while ((SNVS_HPCR & SNVS_HPCR_PI_EN)) {}

        // interrupt frequency 1Hz
        SNVS_HPCR = SNVS_HPCR_PI_FREQ(0b1111);

        // enable periodic snvs interrupt
        SNVS_HPCR |= SNVS_HPCR_PI_EN;
        while (!(SNVS_HPCR & SNVS_HPCR_PI_EN)) {}

        attachInterruptVector(IRQ_SNVS_IRQ, SNVS_isr);
        NVIC_SET_PRIORITY(IRQ_SNVS_IRQ, 255); // lowest
        NVIC_ENABLE_IRQ(IRQ_SNVS_IRQ);
    }
}

#include "memoryTool.h"
#include "Stream.h"

namespace MemoryTool
{
    namespace // private
    {
        Stream* stream = &Serial;

        // used linker symbols
        extern "C" {
        unsigned long _stext;
        unsigned long _etext;
        unsigned long _sdata;
        unsigned long _edata;
        unsigned long _sbss;
        unsigned long _ebss;
        unsigned long _estack;
        unsigned long _heap_end;
        unsigned long _heap_start;
        }

        // pretty print a pointer into a char buffer
        void ptr2hex(const uintptr_t p, char* buf, size_t bufSize)
        {
            uint16_t low  = p & 0x0000'FFFF;
            uint16_t high = (p >> 16) & 0x0000'FFFF;
            snprintf(buf, bufSize, "0x%04X'%04X", high, low);
        }
    }

    void doPrintT4(const char* name, const void* startPtr, uint32_t elemSize, uint32_t elements = 1)
    {
        char startAddr[20], endAddr[20], location[30] = "unknown";

        uint32_t size   = elemSize * elements;
        uintptr_t start = (uintptr_t)startPtr;
        uintptr_t end   = start + size - 1;

#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41)
        uint32_t flashS      = 0x6000'0000;
        uint32_t initVarsS   = (uint32_t)&_sdata;
        uint32_t zeroedVarsS = (uint32_t)&_sbss;
        uint32_t stackEnd    = (uint32_t)&_estack;
        uint32_t heapS       = (uint32_t)&_heap_start;
        uint32_t ramS        = 0x2020'0000;

        if (start >= flashS)
        {
            snprintf(location, sizeof(location), "FLASH");
        }
        else if (start >= heapS)
        {
            snprintf(location, sizeof(location), "HEAP (on RAM-2)");
        }
        else if (start >= ramS)
        {
            snprintf(location, sizeof(location), "DMAMEM (not initialized, RAM-2)");
        }
        else if (start >= stackEnd)
        {
            snprintf(location, sizeof(location), "STACK (on RAM-1)");
        }
        else if (start >= zeroedVarsS)
        {
            snprintf(location, sizeof(location), "DTCM (initialized, RAM-1)");
        }
        else if (start >= initVarsS)
        {
            snprintf(location, sizeof(location), "DTCM (zeroed, RAM-1)");
        }
        else
        {
            snprintf(location, sizeof(location), "ITCM (Code copied to RAM-1)");
        }

#elif defined(ARDUINO_TEENSYLC) || defined(ARDUINO_TEENSY31) || defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY36)

        uint32_t initVarsS   = (uint32_t)&_sdata;
        uint32_t zeroedVarsS = (uint32_t)&_sbss;
        //uint32_t stackEnd    = (uint32_t)&_estack;
        uint32_t heapS = (uint32_t)&_heap_start;

        char dummy;

        stream->printf("dummy:      0x%08X\n", &dummy);
        stream->printf("heap:   0x%08X\n", heapS);
        stream->printf("initVars:   0x%08X\n", initVarsS);
        stream->printf("zeroVars:   0x%08X\n", zeroedVarsS);

        if (start >= (uintptr_t)&dummy)
        {
            snprintf(location, sizeof(location), "STACK");
        }
        else if (start >= heapS)
        {
            snprintf(location, sizeof(location), "HEAP");
        }
        else if (start >= zeroedVarsS)
        {
            snprintf(location, sizeof(location), "RAM (zeroed)");
        }
        else if (start >= initVarsS)
        {
            snprintf(location, sizeof(location), "RAM (initialized)");
        }
        else
        {
            snprintf(location, sizeof(location), "FLASH");
        }

#endif

        // stream->printf("dataStart:   0x%08X\n", dataStart);
        // stream->printf("ramStart:   0x%08X\n", ramstart);

        ptr2hex(start, startAddr, sizeof(startAddr));
        ptr2hex(end, endAddr, sizeof(endAddr));

        stream->printf("%s\n", name);
        stream->printf("  Start address: %s\n", startAddr);
        if (elemSize == 0)
        {
            stream->printf("  End address:   n.a\n");
            stream->printf("  Size:          n.a.\n");
        }
        else
        {
            stream->printf("  End address:   %s\n", endAddr);
            stream->printf("  Size:          %d Bytes\n", size);
        }
        stream->printf("  Location:      %s\n", location);

        stream->println();
    }

    void doPrint(const char* name, const void* startPtr, uint32_t elemSize, uint32_t elements = 1)
    {
        // char startAddr[20], endAddr[20], location[30] = "unknown";

        // uint32_t size   = elemSize * elements;
        // uintptr_t start = (uintptr_t)startPtr;
        // uintptr_t end   = start + size - 1;

        // char curStackEndVar;
        // uint32_t curStackEnd = (uint32_t)&curStackEndVar;

        // ptr2hex(start, startAddr, sizeof(startAddr));
        // ptr2hex(end, endAddr, sizeof(endAddr));

        // if (start >= flashStart && start < flashEnd)
        // {
        //     snprintf(location, sizeof(location), "FLASH");
        // }
        // else if (start > dataStart && start < dataEnd)
        // {
        //     snprintf(location, sizeof(location), "RAM (initialized)");
        // }
        // else if (start > bssStart && start < bssEnd)
        // {
        //     snprintf(location, sizeof(location), "RAM (not initialized)");
        // }
        // else if (start > bssEnd && start < curStackEnd)
        // {
        //     snprintf(location, sizeof(location), "HEAP");
        // }
        // else if (start > curStackEnd && start < stackEnd)
        // {
        //     snprintf(location, sizeof(location), "STACK");
        // }

        // stream->printf("%s\n", name);
        // stream->printf("  Start address: %s\n", startAddr);
        // if (elemSize == 0)
        // {
        //     stream->printf("  End address:   n.a\n");
        //     stream->printf("  Size:          n.a.\n");
        // }
        // else
        // {
        //     stream->printf("  End address:   %s\n", endAddr);
        //     stream->printf("  Size:          %d Bytes\n", size);
        // }
        // stream->printf("  Location:      %s\n", location);

        // stream->println();
    }

    void begin(Stream& _stream)
    {
        stream = &_stream;

        // stream->printf("flashStart: 0x%08X\n", flashStart);
        // stream->printf("flashEnd:   0x%08X\n", flashEnd);
        // stream->printf("stackEnd:   0x%08X\n", stackEnd);
        // stream->printf("dataStart:   0x%08X\n", dataStart);
        // stream->printf("ramStart:   0x%08X\n", ramstart);
        // stream->printf("ramend:   0x%08X\n", ramend);
        // stream->printf("itcmStart:   0x%08X\n", itcm_start);
        // stream->printf("itcmEnd:   0x%08X\n", itcm_end);
        // stream->printf("bssStart:   0x%08X\n", bssStart);

        //stream->printf("itcmEnd:   0x%08X\n", itcm_end);

        //stream->printf("dataEnd:     0x%08X\n", data);
    }

}
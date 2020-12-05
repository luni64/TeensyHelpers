#include "memoryTool.h"
#include "Stream.h"


namespace MemoryTool
{
    namespace // private
    {
        Stream* stream = &Serial;

        // used linker symbols
        extern "C" {
        unsigned long _etext;
        unsigned long _sdata;
        unsigned long _edata;
        unsigned long _sbss;
        unsigned long _ebss;
        unsigned long _estack;
        }

        uint32_t flashStart = 0;
        uint32_t flashEnd   = (uint32_t)&_etext;
        uint32_t dataStart  = (uint32_t)&_sdata;
        uint32_t dataEnd    = (uint32_t)&_edata;
        uint32_t bssStart   = (uint32_t)&_sbss;
        uint32_t bssEnd     = (uint32_t)&_ebss;
        uint32_t stackEnd   = (uint32_t)&_estack;

        // pretty print a pointer into a char buffer
        void ptr2hex(const uintptr_t p, char* buf, size_t bufSize)
        {
            uint16_t low  = p & 0x0000'FFFF;
            uint16_t high = (p >> 16) & 0x0000'FFFF;
            snprintf(buf, bufSize, "0x%04X'%04X", high, low);
        }
    }

    void doPrint(const char* name, const void* startPtr, uint32_t elemSize, uint32_t elements = 1)
    {
        char startAddr[20], endAddr[20], location[30] = "unknown";

        uint32_t size   = elemSize * elements;
        uintptr_t start = (uintptr_t)startPtr;
        uintptr_t end   = start + size - 1;

        char curStackEndVar;
        uint32_t curStackEnd = (uint32_t)&curStackEndVar;

        ptr2hex(start, startAddr, sizeof(startAddr));
        ptr2hex(end, endAddr, sizeof(endAddr));

        if (start >= flashStart && start < flashEnd)
        {
            snprintf(location, sizeof(location), "FLASH");
        }
        else if (start > dataStart && start < dataEnd)
        {
            snprintf(location, sizeof(location), "RAM (initialized)");
        }
        else if (start > bssStart && start < bssEnd)
        {
            snprintf(location, sizeof(location), "RAM (not initialized)");
        }
        else if (start > bssEnd && start < curStackEnd)
        {
            snprintf(location, sizeof(location), "HEAP");
        }
        else if (start > curStackEnd && start < stackEnd)
        {
            snprintf(location, sizeof(location), "STACK");
        }

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

    void begin(Stream& _stream)
    {
        stream = &_stream;

        // stream->printf("flashStart: 0x%08X\n", flashStart);
        // stream->printf("flashEnd:   0x%08X\n", flashEnd);
        // stream->printf("stackEnd:   0x%08X\n", stackEnd);
        // stream->printf("ramStart:   0x%08X\n", ramStart);
        // stream->printf("ramEnd:     0x%08X\n", ramEnd);
    }

}
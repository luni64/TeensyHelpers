#include "usb_serial.h"
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace MemoryTool
{
    void setStream(Stream&);

    // don't use in user code
    void doPrint(const char* name, const void* start, uint32_t elemSize, uint32_t elements);


    // print info about single variables
    template <typename T>
    void printInfo(const char* name, T& var)
    {
        doPrint(name,(void*)&var, sizeof(T), 1);
    }

    // print info about arrays
    template <size_t nrOfElements, typename T>
    void printInfo(const char* name, T (&var)[nrOfElements])
    {
        doPrint(name, &var, sizeof(T), nrOfElements);
    }

    // print info about functions
    template <typename F>
    void printFInfo(const char* name, F& f)
    {
        doPrint(name, (void*)&f, 0, 0);
    }
}

// helpers to automatically extract variable name
#define printMemoryInfo(var) MemoryTool::printInfo(#var, (var))
#define printFuncInfo(f) MemoryTool::printFInfo(#f, (f))

#include "attachInterruptEx.h"
#include "core_pins.h"
#include <array>

namespace
{
    constexpr unsigned num_pins = CORE_NUM_DIGITAL;

    // plain array to store the 'std::function pointers' to the callbacks
    std::function<void()> callbacks[num_pins];

    // relay function to be attached to the pin interrupt
    template <unsigned nr>
    void relay()
    {
        callbacks[nr]();
    }

    // function to generate a std::array intialized with the corresponding relay functions
    template <std::size_t... I>
    constexpr std::array<void (*)(), num_pins> MakeRelays(std::index_sequence<I...>)
    {
        return std::array<void (*)(), num_pins>{relay<I>...};
    }

    // the actual array of relay functions. Generated at compile time.
    constexpr auto relays = MakeRelays(std::make_index_sequence<num_pins>{});
} // namespace

void attachInterruptEx(unsigned pin, std::function<void(void)> callback, int mode)
{
    callbacks[pin] = callback;               // store the callback function in its array
    attachInterrupt(pin, relays[pin], mode); // attach the relay function to the pin interrupt
}
#pragma once

#include <functional>

extern void attachInterruptEx(unsigned pin, std::function<void(void)> callback, int mode);

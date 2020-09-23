#pragma once

using yieldFunc_t = void(*)();

extern void attachYieldFunc(yieldFunc_t yieldFunction);
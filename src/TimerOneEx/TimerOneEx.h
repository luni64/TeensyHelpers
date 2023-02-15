#pragma once

#include "TimerOne.h"

template <typename T>
class TimerOneEx : public TimerOne
{
 public:
    using callback_t = void (T::*)();

    TimerOneEx()
    {
        callback = nullptr;
        object   = nullptr;
    }

    void attachInterrupt(callback_t cb, T* obj) 
    {
        callback = cb;
        object   = obj;
        TimerOne::attachInterrupt(relay);
    }

 private:
    static void relay()
    {
        if (object) (*object.*callback)();
    }

    static callback_t callback;
    static T* object;

    using TimerOne::attachInterrupt; // don't shadow the orignal version
};

//instantiate static variables

template <typename T>
T* TimerOneEx<T>::object = nullptr;

template <typename T>
typename TimerOneEx<T>::callback_t TimerOneEx<T>::callback = nullptr;

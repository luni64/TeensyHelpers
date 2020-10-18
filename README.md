This repository contains some helper functions and classes for the PJRC Teensy boards:

# Content
- [IntervalTimerEx](#intervaltimerex)\
    Subclasses the standard IntervalTimer to allow passing state to callbacks. You can choose between attaching `std::function<void()>` callbacks or the traditional void pointer pattern.

- [attachInterruptEx](#attachinterruptex)\
    Overloads the attachInterrupt function to allow attaching `std::function<void()>` callbacks.

- [pinModeEx](#pinModeEx)
  Overloaded version of the pinMode function which can set the pin mode for a arbitrary long list of pins.

- [attachYieldFunc](#attachyieldfunc)
  Add your own function to the yield call stack

- [TeensySystemClock](#teensysystemclock)
  Use the new c++11 chrono::system_clock with T4.x. Uses the T4.x RTC to feed the system_clock. Works with/without battery and provides a clock resolution of 1/32678kHz = 30.5µs. 

**All functions and classes use the underlying Teensyduino mechanisms and bookkeeping. They can mixed with the standard ones.**

# Installation:
I didn't bother to make this a library. To use it just copy the `*.h` and  `*.cpp` files from the corresponding  subfolders of `/src` into your sketch to use them.


-----

# IntervalTimerEx

`IntervalTimerEx` subclasses the stock `IntervalTimer` so that you can
pass state to the timer callbacks. IntervalTimerEx provides either a traditional `void(*)(void*)` pattern or, more modern `std::function<>` typed callbacks. You can choose which to use with a preprocessor switch in `IntervalTimer.h`

- **`std::function` approach:**\
Usage is exactly the same as with the normal IntervalTimer. However, it accepts more or less anything witch can be called (functions, member functions, lambdas, functors) as callbacks.

- **Traditional void pointer approach:**\
Might be more familiar to some users. Uses less resources, doesn't use dynamic memory. You define the state which is passed to the callback function in `IntervalTimer.begin()`.

# Examples:

## **std::function approach**

```c++
#include "IntervalTimerEx.h"

// plain vanilla void(*)() callback
void myCallback_0(){
    Serial.print("myCallback_0 ");
    Serial.println(millis());
}

// need to manipulate the calling timer in the callback?
void myCallback_1(IntervalTimer* caller){
    Serial.print("--> myCallback_1 ");
    Serial.println(millis());

    caller->end(); // e.g. do a one shot timer
}

IntervalTimerEx t1, t2;

void setup(){
    t1.begin(myCallback_0, 150'000);
    t2.begin([] { myCallback_1(&t2); }, 1'000'000);  // attach callback using a lambda
}

void loop(){
}
```

### Embedding in a user class

```c++
#include "IntervalTimerEx.h"

class DeepThougth
{
 public:
    void begin()
    {
        answer = 42;
        t1.begin([this] { ISR(); }, 7.5E6);  // 7.5s
    }

 protected:
    IntervalTimerEx t1;
    int answer;

    void ISR()
    {
        Serial.printf("The answer is %d\n", answer);
    }
};

//----------------------------

DeepThougth deepThougth;

void setup(){
    deepThougth.begin();
}

void loop(){
}
```

## Traditional void pointer approach:

Comment out the preprocessor switch `#define USE_CPP11_CALLBACKS` in  `IntervalTimerEx.h` to use this pattern.

### One shot timer
```c++
#include "IntervalTimerEx.h"

// implement a one shot timer by using the timer address
// passed to the callback
void myCallback_1(void* state){
    IntervalTimer* timer = (IntervalTimer*)state;
    Serial.printf("Called @\t%d ms\n", millis());
    timer->end();
}
//------------------

IntervalTimerEx t1;

void setup(){
    while(!Serial);
    Serial.printf("Triggered @\t%d ms\n", millis());
    t1.begin(myCallback_1, &t1, 200'000); // pass the address of the timer to the callback
}

void loop(){
}
```
Output:
```
Triggered @	388 ms
Called @	588 ms
```


### Embedding an IntervalTimer in a user class
```c++
class DeepThougth
{
 public:
    void begin()
    {
        answer = 42;
        t1.begin(ISR, this, 7.5E6); // 7.5s
    }

 protected:
    IntervalTimerEx t1;
    int answer;

    static void ISR(void* obj)
    {
        DeepThougth* THIS = (DeepThougth*)obj;
        Serial.printf("The answer is %d\n", THIS->answer);
    }
};

DeepThougth deepThougth;

void setup(){
    deepThougth.begin();
}

void loop(){
}
```

# attachInterruptEx

You can use `attachInterruptEx` in exactly the same as you use the standard `attachInterrupt` function. However, it accepts more or less anything witch can be called (functions, member functions, lambdas, functors) as callbacks.

## Examples

Shows how to encapsulate and setup a pin interrupt callback function in a user class. The EdgeCounter class simply counts all edges (rising/falling) it sees on a digital pin.  The pin to be monitored is settable with the `begin()` function.

```c++
#include "attachInterruptEx.h"

class EdgeCounter
{
 public:
    void begin(unsigned pin)
    {
        counter = 0;
        pinMode(pin, INPUT_PULLUP);
        attachInterruptEx(pin, [this] { this->ISR(); }, CHANGE);
    }

    unsigned getCounter() { return counter; }

 protected:
    void ISR(){
        counter++;
    }

    unsigned counter;
};

//-------------------------

EdgeCounter ec1, ec2;

void setup(){
    ec1.begin(0);
    ec2.begin(1);
}

void loop(){
    Serial.printf("Detected edges: Pin1: %u Pin2:%u\n", ec1.getCounter(), ec2.getCounter());
    delay(250);
}

```

# pinModeEx
One often has to define the pin mode for a bunch of pins which can be a bit tedious. In the folder `src/pinModeEx` you find an overloaded version of the `pinMode` function which allows to set the mode for an arbitrary large list of pins.

Usage:
```c++
#include "pinModeEx.h"

const int pinA = 3, pinB = 17, switch1 = 3, switch2 = 4;

void setup(){
    pinMode({pinA, pinB, 17, LED_BUILTIN}, OUTPUT);  // set a bunch of pins to mode OUTPUT...
    pinMode({switch1, switch2}, INPUT_PULLUP);       // others to INPUT
}
```

# attachYieldFunc()

Sometimes your have code that needs to be called by the user as often as possible. Prominent examples are AccelStepper where you are required to call `run()` at high speed. Using the debounce library, you need to call `update()`. The PID library requires a frequent call to `Compute()` and so on.

Usually, the user of these libraries just calls these functions in loop which works fine for simple code. As soon as there is longer running code or some delays in loop the call rate of these functions can get unacceptably low and the libraries don't work as expected.

### yield()
Teensyduino provides a yield() function which is called before each call to loop(), during delay() and probably during a lot of other long running core functions while they spin.

If you want to add your own functions to be called from yield without completely overriding it, you can use `attachYieldFunc(callback)` which is defined in the `src/attachYieldFunc` folder of the repository.
Here an example which will call `myCallback()` from yield to toggle pin 0 in the background. The `delay(250)` in loop does not disturb the high frequency background toggling since `delay()` calls yield while it spins.

```c++
#include "attachYieldFunc.h"

void myCallback(){ //will be called from yield
  digitalToggleFast(0);
}

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, OUTPUT);

  attachYieldFunc(myCallback); // attach our callback to the yield stack
}

void loop(){
  digitalToggleFast(LED_BUILTIN);
  delay(250);
}
```

# TeensySystemClock

This extension enables the new (>c++11) `chrono::system_clock`. Usually the system_clock is fed by using some operating system calls. In embedded systems one simply needs to define the static function `chrono::system_clock::now()` to enable the clock.

The tiny code in the TeensySystemClock folder uses the built in RTC of the T4.x boards to feed the system_clock. It doesn't interfere with the normal use of the RTC functions and works with or without battery. Other than the usual Teensy RTC functions the system_clock uses the full resolution of the RTC which is 1/32768 kHz = 30.5 µs.

Example:

```c++
#include <chrono>
using namespace std::chrono;

typedef system_clock::time_point timePoint;               // save typing...

void setup(){
    while(!Serial){}
}

void loop()
{
    timePoint start = system_clock::now();                // generate two time points 1234ms apart
    delay(1234);
    timePoint stop = system_clock::now();

    nanoseconds  dt    = stop - start;                    // system clock has an underlying ns counter
    milliseconds dt_ms = duration_cast<milliseconds>(dt); // cast dt to milliseconds (float)

    Serial.printf("stop-start: %u ms\n", dt_ms.count());

    time_t t = system_clock::to_time_t(start);            // convert the time_point 'start' to usual time_t
    Serial.printf("Current Time: %s\n", ctime(&t));       // pretty print date/time
}
```

Which prints out:
```
stop-start: 1234 ms
Current Time: Sun Oct 18 21:01:37 2020

stop-start: 1234 ms
Current Time: Sun Oct 18 21:01:38 2020

stop-start: 1234 ms
Current Time: Sun Oct 18 21:01:39 2020

stop-start: 1234 ms
Current Time: Sun Oct 18 21:01:40 2020
...
```

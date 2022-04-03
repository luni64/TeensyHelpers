This repository contains some helper functions and classes for the PJRC Teensy boards:

# Content
- [IntervalTimerEx](#intervaltimerex)\
    Subclasses the standard IntervalTimer to allow passing state to callbacks. You can choose between attaching `std::function<void()>` callbacks or the traditional void pointer pattern.

- [attachInterruptEx](#attachinterruptex)\
    Overloads the attachInterrupt function to allow attaching `std::function<void()>` callbacks.

- [pinModeEx](#pinModeEx)\
  Overloaded version of the pinMode function which can set the pin mode for a arbitrary long list of pins.

- [attachYieldFunc](#attachyieldfunc)\
  Add your own function to the yield call stack

- [teensy_clock](#teensy_clock)\
  Use the new c++11 ```std::chrono``` time system to implement a
  `std::chrono` compliant clock which uses the cycle counter as time base. It counts time in 1.667ns steps (1/F_CPU)  since 0:00h 1970-01-01.

- [instanceList](#instancelist)\
  Helper to automatically maintain a list of all active objects of a class and call
  member functions on all of these objects. Helpful for example if you need to periodically tick all existing objects of a class.


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

class DeepThought
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

DeepThought deepThought;

void setup(){
    deepThought.begin();
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
class DeepThought
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
        DeepThought* THIS = (DeepThought*)obj;
        Serial.printf("The answer is %d\n", THIS->answer);
    }
};

DeepThougth deepThought;

void setup(){
    deepThought.begin();
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

# teensy_clock

This extension implements a clock compliant to the new (>c++11) `chrono::system_clock`.

It uses a 64bit extension of the cycle counter where rollover is handled  by the periodic timer of the SNVS_HPCR module.

The teensy_clock can be synced to the the built in RTC. It doesn't interfere with the normal use of the RTC functions and works with or without battery. Other than the usual Teensy RTC functions which are based on full seconds, the teensy_clock uses the full resolution of the cycle counter which is 1/600MHz = 1.6667 ns for a T4@ 600MHz.

A full description of the code can be found in the Teensy User WIKI:
https://github.com/TeensyUser/doc/wiki/fun-with-modern-cpp#start


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
    timePoint start = teensy_clock::now();                // generate two time points 1234ms apart
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

Use the system_clock to wait...

```c++
#include <chrono>
using namespace std::chrono;

void setup(){
}

void loop()
{
    auto start = teensy_clock::now();  // current time point

    while (teensy_clock::now() < (start + 1h + 30min)) // use chrono literals
    {
        yield(); // waiting...
    }

    Serial.println("done");
}
```

# instanceList

This helper class automatically maintains a list of all currently existing instances of a class regardless if they are constructed on the stack, the heap or in global space. The list of instances is accessible using standard c++ iterators.

Possible use cases are classes where you need to periodically call a function on all objects.

## Example
Here a very simple example which demonstrates the usage of the ObjectListHelper class.

Let's assume we want to define a `Blinker` class which periodically toggles a pin. Pin number and toggling period will be set in the constructor.

To toggle the pin we implement a `blink()` member function. Whenever `blink()` is called, it simply checks an `elapsedMicros` variable to determine if it is time to toggle the pin. To get good performance, it is important to call `blink()` as often as possible.


```c++
#include "instanceList.h"

class Blinker
{
 public:
    Blinker(int _pin, unsigned ms)
        : pin(_pin), period(ms * 1000)
    {
        pinMode(pin, OUTPUT);
    }

    void blink()
    {
        if (t > period)
        {
            digitalToggleFast(pin);
            t -= period;
        }
    }

 private:
    elapsedMicros t;
    unsigned pin, period;
};
```

Usage
```c++
Blinker b(0,10); // toggles pin 0 every 10ms

void setup()
{
}

void loop()
{
    b.blink();
}
```
So far, so good. However, if you need a lot of those Blinkers, the code can get messy quickly. Also, those Blinkers might as well be defined in a different file or in some functions and it can get difficult to keep track of  all the existing Blinkers.
```c++
Blinker b1(0,10);
Blinker b2(7,100);
Blinker b3(2,24);
Blinker b4(13,500);

void setup()
{
}

void loop()
{
    b1.blink();
    b2.blink();
    b3.blink();
    b4.blink();
}
```
Of course, it would be a lot simpler if one could call the `blink()` function for all currently existing Blinker instances at once. Using the `instanceList` this can be done easily. All we need to do is derive the Blinker class from `instanceList`. To blink all instances we define a static function `tick()` which
uses the list of instances provides and calls blink on each of them:
```c++
class Blinker : protected InstanceList<Blinker>  // <- derive from InstanceList
{
 public:
    Blinker(int _pin, unsigned ms)
        : pin(_pin), period(ms * 1000)
    {
        pinMode(pin, OUTPUT);
    }

    void blink()
    {
        if (t > period)
        {
            digitalToggleFast(pin);
            t -= period;
        }
    }

    static void tick()
    {
        for (Blinker& b : instanceList) // for each Blinker b in the instance list
        {
            b.blink();
        }
    }

 private:
    elapsedMicros t;
    unsigned pin, period;
};
```

Usage:
```c++
Blinker b0(0, 10);
Blinker b1(1, 60);
Blinker b2(2, 24);
Blinker b3(3, 150);
Blinker LED(13, 500);

void setup()
{
}

void loop()
{
    Blinker::tick();
}
```
Now, we can add and remove Blinkers without having to to update the calls to blink() accordingly.

You can use the instanceList for other things as well. Assume you want to print out information about all currently exisiting Blinker instances. Just add the following member function to the Blinker class:

```c++
static void InstanceInfo()
{
  for (Blinker& b : instanceList) // for each object in objectList
  {
     Serial.printf("pin: %2d, period (µs): %6d, address: %p\n", b.pin, b.period, &b);
  }
}
```
Then, you can print the information by:

```c++
Blinker b0(0, 10);
Blinker b1(1, 60);
Blinker b2(2, 24);
Blinker b3(3, 150);
Blinker LED(13, 500);

void setup()
{
    while(!Serial){}
    Blinker::InstanceInfo();
}

void loop()
{
    Blinker::tick();
}
```

which prints:
```
pin: 13, period (µs): 500000, address: 0x20001864
pin:  3, period (µs): 150000, address: 0x20001908
pin:  2, period (µs):  24000, address: 0x200018f8
pin:  1, period (µs):  60000, address: 0x200018e8
pin:  0, period (µs):  10000, address: 0x200018d8
```





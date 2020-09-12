This repo contains extended versions for the Teensy IntervalTimer class and the attachInterrupt function. In both cases, instead of the standard `void(*)()` callbacks `std::function` type callbacks can be attached (see examples below). It also contains an overloaded version of the pinMode function witch can set the pin mode for a arbitray long list of pins. 

**All functions and classes use the underlying Teensyduino mechanisms and bookkeeping. They can mixed with the standard ones.**

### Installation:
I didn't bother to make this a library. To use it just copy the `*.h` and  `*.cpp` files from the corresponding  subfolders of `/src` into your sketch to use them.

### Content
- [IntervalTimerEx](#intervaltimerex)
- [attachInterruptEx](#attachinteruptex)
- [pinModeEx](#pinModeEx)

-----

## IntervalTimerEx

Usage is exactly the same as with the normal IntervalTimer. However, it accepts more or less anything witch can be called (functions, member functions, lambdas, functors) as callbacks.

## Examples:
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

class Blinker
{
 public:
    void begin(float seconds)
    {
        pinMode(LED_BUILTIN, OUTPUT);
        timer.begin([this] { this->blink(); }, seconds * 1E6); // attach member function
    }

 protected:
    IntervalTimerEx timer;

    void blink()
    {
        digitalToggleFast(LED_BUILTIN);
    }
};

//----------------------------

Blinker blinker;

void setup(){
    blinker.begin(0.1);  // start the blinker
}

void loop(){
}
```

# attachInterruptEx

You can use it in exactly the same as you use the normal attachInterrupt function. However, it accepts more or less anything witch can be called (functions, member functions, lambdas, functors) as callbacks.

## Examples

Shows how to embed and setup a pin interrupt callback function in a user class. The EdgeCounter class simply counts the edges it sees on a digital pin.  The pin number settable with the begin function.

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

EdgeCounter ec1,ec2;

void setup(){
    ec1.begin(0);
    ec2.begin(1);
}

void loop(){
    Serial.printf("Detected edges: Pin1: %u Pin2:%u\n", ec1.getCounter(), ec2.getCounter());
    delay(250);
}

```

## pinModeEx
One often has to define the pin mode for a bunch of pins which can be a bit tedious. In the folder `src/pinModeEx` you find an overloaded version of the `pinMode` function which allows to set the mode for an arbitrary large list of pins.

Usage:
```c++
#include "pinModeEx.h"

const int pinA = 3, pinB = 17, switch1 = 3, switch2 = 4;

void setup()
{
    pinMode({pinA, pinB, 17, LED_BUILTIN}, OUTPUT);  // set a bunch of pins to mode OUTPUT...
    pinMode({switch1, switch2}, INPUT_PULLUP);       // others to INPUT
}
```

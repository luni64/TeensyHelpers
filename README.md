This repo contains some useful helper functions and classes for the PJRC Teensy boards:

# Content
- [IntervalTimerEx](#intervaltimerex)\
    Subclasses the standard IntervalTimer to allow passing state to callbacks. You can choose between attaching `std::function<void()>` callbacks or the traditional void pointer pattern.
- [attachInterruptEx](#attachinteruptex)\
    Overloads the attachInterrupt function to allow attaching `std::function<void()>` callbacks.

- [pinModeEx](#pinModeEx)
  Overloaded version of the pinMode function witch can set the pin mode for a arbitrary long list of pins.

**All functions and classes use the underlying Teensyduino mechanisms and bookkeeping. They can mixed with the standard ones.**

# Installation:
I didn't bother to make this a library. To use it just copy the `*.h` and  `*.cpp` files from the corresponding  subfolders of `/src` into your sketch to use them.


-----

# IntervalTimerEx

The header contains a preprocessor switch to choose between the traditional void pointer pattern to pass state to callbacks and the more modern `std::function` approach.

- **`std::function` approach:**\
Usage is exactly the same as with the normal IntervalTimer. However, it accepts more or less anything witch can be called (functions, member functions, lambdas, functors) as callbacks.

- **Traditional void pointer approach:**\
Might be more familiar to some users. Uses less resources, doesn't use dynamic memory.

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

void setup()
{
    deepThougth.begin();
}

void loop()
{
}
```

## Traditional void pointer approach:

Uncomment the preprocessor switch in the `IntervalTimerEx.h` header.

### Embedding in a user class
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

    static void ISR(void* state)
    {
        DeepThougth* THIS = (DeepThougth*)state;
        Serial.printf("The answer is %d\n", THIS->answer);
    }
};

DeepThougth deepThougth;

void setup()
{
    deepThougth.begin();
}

void loop()
{
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

# pinModeEx
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

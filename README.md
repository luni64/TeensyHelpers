## IntervalTimerEx

### Examples: 
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

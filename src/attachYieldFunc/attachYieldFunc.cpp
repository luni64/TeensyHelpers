#include "attachYieldFunc.h"
#include "Eventresponder.h"


void attachYieldFunc(yieldFunc_t _yieldFunc) // pass a pointer to the function you want to be called from yield
{
  static yieldFunc_t yieldFunc = _yieldFunc; // store the passed in function pointer
  static EventResponder er;                  // define a new EventResponder which will handle the calls.

  er.attach([](EventResponderRef r) {        // we can not directly attach our function to the responder since we need to retrigger the repsonder
    yieldFunc();                             // instead we attach an inline defined relay function as lambda expression
    r.triggerEvent();                        // the relay calls our function and afterwards triggers the responder to schedule the next call.
  });

  er.triggerEvent();                         // to start the call chain we need to trigger the responder once
}
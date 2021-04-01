#include "Arduino.h"
#include "MicroModTeensy.h"

using namespace MMT;

void setup()
{
  pinMode(G0,OUTPUT);
}

void loop()
{
  digitalWriteFast(LED_BUILTIN,!digitalReadFast(LED_BUILTIN));
  delay(500);
}

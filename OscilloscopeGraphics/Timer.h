#ifndef __TIMER__
#define __TIMER__

#include <Arduino.h>

namespace osc {
class Timer {
  unsigned long begin;
  unsigned long end;

 public:
  Timer() { Serial.begin(9600); }
  void start() { begin = micros(); }
  void stop() { end = micros(); }
  void print(char* id) {
    Serial.print(id);
    Serial.print(": ");
    Serial.println(end - begin);
  }
};
}  // namespace osc

#endif

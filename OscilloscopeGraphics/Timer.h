#ifndef __TIMER__
#define __TIMER__

#include <Arduino.h>

namespace osc {
class Timer {
  unsigned long begin;
  unsigned long total;

 public:
  Timer() {
    reset();
    Serial.begin(9600);
  }
  void start() { begin = micros(); }
  void stop() { total += micros() - begin; }
  void reset() { total = 0; }
  void print(char* id) {
    Serial.print(id);
    Serial.print(": ");
    Serial.println(total);
  }
};
}  // namespace osc

#endif

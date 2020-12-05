#ifndef __CONSUMER__
#define __CONSUMER__

#define MAX_LINES 256

#include "types.h"

namespace osc {

template <typename T>
class Consumer {
  long speed;
  Buffer<Line<T>> lines;

 public:
  Consumer(unsigned long speed = 115200)
      : speed(speed), lines(MAX_LINES * sizeof(T) * 4) {}

  void start() {
    Serial.begin(speed);
    Serial.setTimeout(0);
  };

  Buffer<Line<T>>* getLines() {
    if (Serial.available() > 0) {
      // TODO: Replace with readBytes()?
      String data = Serial.readString();
      parseData(data.c_str(), lines);
    }
    return &lines;
  };

 protected:
  virtual void parseData(const char* data, Buffer<Line<T>>& lines) = 0;
};

}  // namespace osc

#endif

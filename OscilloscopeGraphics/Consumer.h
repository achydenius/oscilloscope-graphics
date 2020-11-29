#ifndef __CONSUMER__
#define __CONSUMER__

#define MAX_LINES 256
#define COORD_BYTES 4

#include "types.h"

namespace osc {
class Consumer {
  long speed;
  Buffer<Line> lines;

 public:
  Consumer(unsigned long speed = 115200)
      : speed(speed), lines(MAX_LINES * COORD_BYTES * 4) {}

  void start();
  Buffer<Line>* getLines();

 private:
  void parseData(const char* data, Buffer<Line>& lines);
};
}  // namespace osc

#endif

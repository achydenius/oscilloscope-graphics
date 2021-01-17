#ifndef __LINE_CONSUMER__
#define __LINE_CONSUMER__

#include "Consumer.h"

#define LINE_BUFFER_LENGTH 256
#define SERIAL_BUFFER_SIZE (LINE_BUFFER_LENGTH * 10)

namespace osc {

class LineConsumer : public Consumer<Line> {
 public:
  LineConsumer(unsigned long speed = 115200)
      : Consumer(SERIAL_BUFFER_SIZE, LINE_BUFFER_LENGTH, speed) {}

  void parseData(const char* data, size_t length, Buffer<Line>* lines) {
    uint16_t* coords = (uint16_t*)data;
    int coordCount = length / 2;

    lines->reset();
    for (int i = 0; i < coordCount; i += 5) {
      lines->add({
          {coords[i], coords[i + 1]},
          {coords[i + 2], coords[i + 3]},
          coords[i + 4],
      });
    }
  }
};

}  // namespace osc

#endif

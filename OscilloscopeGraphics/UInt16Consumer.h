#ifndef __UINT16_CONSUMER__
#define __UINT16_CONSUMER__

#include "Consumer.h"

#define MAX_LINES 256

namespace osc {

class UInt16Consumer : public Consumer<uint16_t> {
 public:
  UInt16Consumer(unsigned long speed = 115200)
      : Consumer(MAX_LINES, 8, speed) {}

  void parseData(const char* data, size_t length,
                 Buffer<Line<uint16_t>>* lines) {
    uint16_t* coords = (uint16_t*)data;

    lines->reset();
    for (unsigned int i = 0; i < length / 2; i += 4) {
      lines->add({{coords[i], coords[i + 1]}, {coords[i + 2], coords[i + 3]}});
    }
  }
};

}  // namespace osc

#endif

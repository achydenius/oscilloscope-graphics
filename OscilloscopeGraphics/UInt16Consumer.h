#ifndef __UINT16_CONSUMER__
#define __UINT16_CONSUMER__

#include "Consumer.h"

namespace osc {

class UInt16Consumer : public Consumer<uint16_t> {
 public:
  UInt16Consumer(unsigned long speed = 115200) : Consumer(speed) {}

  void parseData(const char* data, Buffer<Line<uint16_t>>& lines) {
    int coordCount = (int)data[0];
    uint16_t* coords = (uint16_t*)&data[4];

    lines.reset();
    for (int i = 0; i < coordCount; i += 4) {
      Line<uint16_t> line;
      line.a.x = coords[i];
      line.a.y = coords[i + 1];
      line.b.x = coords[i + 2];
      line.b.y = coords[i + 3];
      lines.add(line);
    }
  };
};

}  // namespace osc

#endif

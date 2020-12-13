#ifndef __POINT_3D_CONSUMER__
#define __POINT_3D_CONSUMER__

#include "Consumer.h"

namespace osc {

class Point3DConsumer : public Consumer<Point3D> {
 public:
  Point3DConsumer(unsigned long speed = 115200) : Consumer(speed) {}

  void parseData(const char* data, Buffer<Line<Point3D>>& lines) {
    int lineCount = (int)data[0];
    uint8_t* coords = (uint8_t*)&data[4];

    lines.reset();
    for (int i = 0; i < lineCount; i++) {
      Line<Point3D> line;

      line.a.x = *(uint16_t*)&coords[0];
      line.a.y = *(uint16_t*)&coords[2];
      line.a.z = *(float*)&coords[4];

      line.b.x = *(uint16_t*)&coords[8];
      line.b.y = *(uint16_t*)&coords[10];
      line.b.z = *(float*)&coords[12];

      coords += 16;
      lines.add(line);
    }
  };
};

}  // namespace osc

#endif

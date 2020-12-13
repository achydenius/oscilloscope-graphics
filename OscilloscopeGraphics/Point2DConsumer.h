#ifndef __POINT_2D_CONSUMER__
#define __POINT_2D_CONSUMER__

#include "Consumer.h"

namespace osc {

class Point2DConsumer : public Consumer<Point2D> {
 public:
  Point2DConsumer(unsigned long speed = 115200) : Consumer(speed) {}

  void parseData(const char* data, Buffer<Line<Point2D>>& lines) {
    int lineCount = (int)data[0];
    uint8_t* coords = (uint8_t*)&data[4];

    lines.reset();
    for (int i = 0; i < lineCount; i++) {
      Line<Point2D> line;

      line.a.x = *(uint16_t*)&coords[0];
      line.a.y = *(uint16_t*)&coords[2];
      line.b.x = *(uint16_t*)&coords[4];
      line.b.y = *(uint16_t*)&coords[6];

      coords += 8;
      lines.add(line);
    }
  };
};

}  // namespace osc

#endif

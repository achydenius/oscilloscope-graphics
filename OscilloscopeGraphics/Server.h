#ifndef __SERVER__
#define __SERVER__

#define MAX_LINES 256
#define COORD_BYTES 4

#include <Arduino.h>

#include "types.h"

namespace osc {
class Server {
  long speed;
  Buffer<Line> lines;

 public:
  Server(unsigned long speed = 115200)
      : speed(speed), lines(MAX_LINES * COORD_BYTES * 4) {}

  void start() {
    Serial.begin(speed);
    Serial.setTimeout(0);
  }

  Buffer<Line>* update() {
    if (Serial.available() > 0) {
      // TODO: Replace with readBytes()?
      String data = Serial.readString();
      parseData(data.c_str(), lines);
    }

    return &lines;
  }

  void parseData(const char* data, Buffer<Line>& lines) {
    int coordCount = (int)data[0];
    float* coords = (float*)&data[4];

    lines.reset();
    for (int i = 0; i < coordCount; i += 4) {
      osc::Line line;
      line.a[0] = coords[i];
      line.a[1] = coords[i + 1];
      line.b[0] = coords[i + 2];
      line.b[1] = coords[i + 3];
      lines.add(line);
    }
  }
};
}  // namespace osc

#endif

#include "Consumer.h"

#include <Arduino.h>

using namespace osc;

void Consumer::start() {
  Serial.begin(speed);
  Serial.setTimeout(0);
}

Buffer<Line>* Consumer::getLines() {
  if (Serial.available() > 0) {
    // TODO: Replace with readBytes()?
    String data = Serial.readString();
    parseData(data.c_str(), lines);
  }

  return &lines;
}

void Consumer::parseData(const char* data, Buffer<Line>& lines) {
  int coordCount = (int)data[0];
  float* coords = (float*)&data[4];

  lines.reset();
  for (int i = 0; i < coordCount; i += 4) {
    Line line;
    line.a.x = coords[i];
    line.a.y = coords[i + 1];
    line.b.x = coords[i + 2];
    line.b.y = coords[i + 3];
    lines.add(line);
  }
}

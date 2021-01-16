#ifndef __CONSUMER__
#define __CONSUMER__

#include "types.h"

namespace osc {

template <typename T>
class Consumer {
  long speed;
  Buffer<Line<T>>* lines;
  char* buffer;

 public:
  Consumer(int maxLines, int bytesPerLine, unsigned long speed) : speed(speed) {
    lines = new Buffer<Line<T>>(maxLines);
    buffer = new char[maxLines * bytesPerLine];
  }

  void start() { Serial.begin(speed); };

  Buffer<Line<T>>* getLines() {
    if (Serial.available()) {
      uint16_t length;
      Serial.readBytes((char*)&length, 2);
      Serial.readBytes(buffer, length);
      parseData(buffer, length, lines);
    }
    return lines;
  };

 protected:
  virtual void parseData(const char* data, size_t length,
                         Buffer<Line<T>>* lines) = 0;
};

}  // namespace osc

#endif

#ifndef __CONSUMER__
#define __CONSUMER__

#include "types.h"

namespace osc {

template <typename T>
class Consumer {
  long speed;
  char* serialBuffer;
  Buffer<T> parsedBuffer;

 public:
  Consumer(int serialBufferSize, int parsedBufferLength, unsigned long speed)
      : speed(speed), parsedBuffer(parsedBufferLength) {
    serialBuffer = new char[serialBufferSize];
  }

  ~Consumer() { delete serialBuffer; }

  void start() { Serial.begin(speed); };

  Buffer<T>& readAndParse() {
    if (Serial.available()) {
      uint16_t length;
      Serial.readBytes((char*)&length, 2);
      Serial.readBytes(serialBuffer, length);
      parseData(serialBuffer, length, parsedBuffer);
    }
    return parsedBuffer;
  };

 protected:
  virtual void parseData(const char* data, size_t length,
                         Buffer<T>& parsedBuffer) = 0;
};

}  // namespace osc

#endif

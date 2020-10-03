#ifndef __RENDERER__
#define __RENDERER__

#include <Arduino.h>

#include "src/cglm/include/cglm/cglm.h"

namespace osc {
class Renderer {
 public:
  virtual void drawLine(vec2& a, vec2& b) = 0;
};

class ArduinoRenderer : public Renderer {
 public:
  // INLINE and DIRECT write modes work only with SAMD51 based boards such as
  // Adafruit Metro M4 (https://www.adafruit.com/product/3382) as they directly
  // access the low-level API.
  enum DACWriteMode { STANDARD, INLINE, DIRECT };

 private:
  const uint8_t resolution;
  const uint8_t xPin, yPin;
  uint32_t maxValue;
  DACWriteMode writeMode;

 public:
  ArduinoRenderer(uint8_t resolution, uint8_t xPin, uint8_t yPin,
                  DACWriteMode mode = DACWriteMode::STANDARD)
      : resolution(resolution), xPin(xPin), yPin(yPin), writeMode(mode) {
    analogWriteResolution(resolution);
    maxValue = pow(2, resolution) - 1;

    // Initialize DAC manually (needed by direct write modes)
    analogWrite(xPin, 0);
    analogWrite(yPin, 0);
  }
  void setWriteMode(DACWriteMode mode);
  void drawLine(vec2& a, vec2& b);

 private:
  void dacWriteAnalogWrite(uint32_t x, uint32_t y);
  void dacWriteInline(uint32_t x, uint32_t y, uint32_t shift);
  void dacWriteDirect(uint32_t x, uint32_t y, uint32_t shift);
};
}  // namespace osc

#endif

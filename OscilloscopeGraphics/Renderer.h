#ifndef __RENDERER__
#define __RENDERER__

#include <Arduino.h>

#include "types.h"

#define INPUT_BITS 16
#define DAC_MAX_RESOLUTION_BITS 12

namespace osc {

// Inline and direct write modes work only with SAMD51 based boards such as
// Adafruit Metro M4 (https://www.adafruit.com/product/3382) as they directly
// access the low-level API.
class Renderer {
 protected:
  const uint8_t resolution;
  const uint8_t xPin, yPin;

 public:
  Renderer(uint8_t resolution, uint8_t xPin, uint8_t yPin)
      : resolution(resolution), xPin(xPin), yPin(yPin) {
    analogWriteResolution(resolution);

    // Initialize DAC manually (needed by direct write modes)
    analogWrite(xPin, 0);
    analogWrite(yPin, 0);
  }

  void drawPoint(Point<uint16_t>& point);
  void drawSolidLine(Line<uint16_t>& line);
  void drawDashedLine(Line<uint16_t>& line);

 protected:
  virtual void dacWrite(uint32_t x, uint32_t y, uint32_t shift) = 0;

 private:
  uint32_t transform(uint16_t value) {
    return value >> (INPUT_BITS - resolution);
  }
  void render(Line<uint16_t>& line,
              void (Renderer::*loop)(int32_t, float, float, float, float, float,
                                     float, uint32_t));
  void solidLoop(int32_t steps, float x, float y, float u, float ix, float iy,
                 float iu, uint32_t shift);
  void dashedLoop(int32_t steps, float x, float y, float u, float ix, float iy,
                  float iu, uint32_t shift);
};

class StandardRenderer : public Renderer {
 public:
  StandardRenderer(uint8_t resolution, uint8_t xPin, uint8_t yPin)
      : Renderer(resolution, xPin, yPin) {}

 protected:
  inline void dacWrite(uint32_t x, uint32_t y, uint32_t shift) {
    analogWrite(xPin, x);
    analogWrite(yPin, y);
  }
};

class InlineRenderer : public Renderer {
 public:
  InlineRenderer(uint8_t resolution, uint8_t xPin, uint8_t yPin)
      : Renderer(resolution, xPin, yPin) {}

 protected:
  inline void dacWrite(uint32_t x, uint32_t y, uint32_t shift) {
    while (!DAC->STATUS.bit.READY0)
      ;
    while (DAC->SYNCBUSY.bit.DATA0)
      ;
    DAC->DATA[0].reg = x << shift;

    while (!DAC->STATUS.bit.READY1)
      ;
    while (DAC->SYNCBUSY.bit.DATA1)
      ;
    DAC->DATA[1].reg = y << shift;
  }
};

class DirectRenderer : public Renderer {
 public:
  DirectRenderer(uint8_t resolution, uint8_t xPin, uint8_t yPin)
      : Renderer(resolution, xPin, yPin) {}

 protected:
  inline void dacWrite(uint32_t x, uint32_t y, uint32_t shift) {
    DAC->DATA[0].reg = x << shift;
    DAC->DATA[1].reg = y << shift;
  }
};

}  // namespace osc

#endif

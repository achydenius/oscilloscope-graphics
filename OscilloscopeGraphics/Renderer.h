#ifndef __RENDERER__
#define __RENDERER__

#include <Arduino.h>
#include <math.h>

namespace osc {
class Renderer {
 public:
  // INLINE and DIRECT write modes work only with SAMD51 based boards such as
  // Adafruit Metro M4 (https://www.adafruit.com/product/3382) as they directly
  // access the low-level API.
  enum DACWriteMode { STANDARD, INLINE, DIRECT };

  struct Window {
    float top, bottom, left, right;
  };

  struct Line {
    float x0, y0, x1, y1;
  };

 private:
  const uint32_t CLIP_INSIDE = 0;
  const uint32_t CLIP_LEFT = 1;
  const uint32_t CLIP_RIGHT = 2;
  const uint32_t CLIP_BOTTOM = 4;
  const uint32_t CLIP_TOP = 8;

  const uint8_t resolution;
  const uint8_t xPin, yPin;
  uint32_t maxValue;
  DACWriteMode writeMode;
  Window viewport;

 public:
  Renderer(uint8_t resolution, uint8_t xPin, uint8_t yPin, DACWriteMode mode)
      : resolution(resolution), xPin(xPin), yPin(yPin), writeMode(mode) {
    analogWriteResolution(resolution);
    maxValue = pow(2, resolution) - 1;
    viewport = {1.0, -1.0, -1.0, 1.0};

    // Initialize DAC manually (needed by direct write modes)
    analogWrite(xPin, 0);
    analogWrite(yPin, 0);
  }
  void setWriteMode(DACWriteMode mode);
  void setViewport(Window& vp);
  void drawPoint(float x, float y);
  void drawLine(float x0, float y0, float x1, float y1);
  bool clipLine(Line* source, Line* target, Window* window);
  void clipAndDrawLine(float x0, float y0, float x1, float y1);
  void drawViewport();

 private:
  uint32_t getClipCode(float x, float y, Window* window);
  void outputLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
  void dacWriteAnalogWrite(uint32_t x, uint32_t y);
  void dacWriteInline(uint32_t x, uint32_t y, uint32_t shift);
  void dacWriteDirect(uint32_t x, uint32_t y, uint32_t shift);
};
}  // namespace osc

#endif

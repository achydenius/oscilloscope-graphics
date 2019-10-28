#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include <Arduino.h>
#include <math.h>

class OscilloscopeGraphics {
  const uint32_t CLIP_INSIDE = 0;
  const uint32_t CLIP_LEFT = 1;
  const uint32_t CLIP_RIGHT = 2;
  const uint32_t CLIP_BOTTOM = 4;
  const uint32_t CLIP_TOP = 8;
  uint8_t resolution;
  uint32_t maxValue;

 public:
  OscilloscopeGraphics(uint8_t resolution) : resolution(resolution) {
    analogWriteResolution(resolution);
    maxValue = pow(2, resolution);
  }
  void plot(uint32_t x, uint32_t y);
  void line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
  void clipLine(float x0, float y0, float x1, float y1);

 private:
  uint32_t getClipCode(float x, float y);
};

#endif

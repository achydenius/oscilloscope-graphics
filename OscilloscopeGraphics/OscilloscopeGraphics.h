#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include <Arduino.h>
#include <math.h>

class OscilloscopeGraphics {
  uint8_t resolution;

 public:
  OscilloscopeGraphics(uint8_t resolution) : resolution(resolution) {
    analogWriteResolution(resolution);
  }
  void plot(uint32_t x, uint32_t y);
  void line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
};

#endif

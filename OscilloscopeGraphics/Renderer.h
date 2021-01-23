#ifndef __RENDERER__
#define __RENDERER__

#include <Arduino.h>

#include "types.h"

#define INPUT_BITS 16
#define DAC_MAX_RESOLUTION_BITS 12

namespace osc {

class Renderer {
 protected:
  const uint8_t resolution;
  const uint8_t xPin, yPin;

 public:
  Renderer(uint8_t resolution, uint8_t xPin, uint8_t yPin)
      : resolution(resolution), xPin(xPin), yPin(yPin) {
    analogWriteResolution(resolution);
  }

  void drawPoint(Point& point);
  void drawLine(Line& line);

 protected:
  virtual void dacWrite(uint32_t x, uint32_t y, uint32_t shift) = 0;

 private:
  uint32_t transform(uint16_t value) {
    return value >> (INPUT_BITS - resolution);
  }
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

// Low-level analog write implementation (Teensy 3.6) from Teensy core library:
// https://github.com/PaulStoffregen/cores/blob/08b835afb8bc4e3adc5b0173b88c20c69abde2a1/teensy3/analog.c#L520-L564
// Inlining the code dramatically improves performance as the write function
// call in rendering loop is bypassed.
typedef int16_t __attribute__((__may_alias__)) aliased_int16_t;
class InlineRenderer : public Renderer {
 public:
  InlineRenderer(uint8_t resolution, uint8_t xPin, uint8_t yPin)
      : Renderer(resolution, xPin, yPin) {}

 protected:
  inline void dacWrite(uint32_t x, uint32_t y, uint32_t shift) {
    SIM_SCGC2 |= SIM_SCGC2_DAC0;
    DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;  // 3.3V VDDA is DACREF_2
    *(volatile aliased_int16_t*)&(DAC0_DAT0L) = x << shift;

    SIM_SCGC2 |= SIM_SCGC2_DAC1;
    DAC1_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;  // 3.3V VDDA is DACREF_2
    *(volatile aliased_int16_t*)&(DAC1_DAT0L) = y << shift;
  }
};

}  // namespace osc

#endif

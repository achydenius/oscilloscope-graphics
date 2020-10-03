#include "ArduinoRenderer.h"

// Transform point from canvas space to output space
#define transform(p) ((uint32_t)(((p * 0.5) + 0.5) * maxValue))

using namespace osc;

void ArduinoRenderer::setWriteMode(DACWriteMode mode) { writeMode = mode; }

/*
 * Draw a line
 *
 * DDA line drawing algorithm implementation:
 * https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
 */
void ArduinoRenderer::drawLine(vec2& a, vec2& b) {
  int32_t x0 = transform(a[0]);
  int32_t y0 = transform(a[1]);
  int32_t x1 = transform(b[0]);
  int32_t y1 = transform(b[1]);

  int32_t dx = x1 - x0;
  int32_t dy = y1 - y0;

  int32_t steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
  float ix = dx / (float)steps;
  float iy = dy / (float)steps;

  float x = x0;
  float y = y0;
  uint32_t shift = 12 - resolution;

  if (writeMode == DACWriteMode::INLINE) {
    for (int32_t i = 0; i <= steps; i++) {
      dacWriteInline(x, y, shift);
      x += ix;
      y += iy;
    }
  } else if (writeMode == DACWriteMode::DIRECT) {
    for (int32_t i = 0; i <= steps; i++) {
      dacWriteDirect(x, y, shift);
      x += ix;
      y += iy;
    }
  } else {
    for (int32_t i = 0; i <= steps; i++) {
      dacWriteAnalogWrite(x, y);
      x += ix;
      y += iy;
    }
  }
}

inline void ArduinoRenderer::dacWriteAnalogWrite(uint32_t x, uint32_t y) {
  analogWrite(xPin, x);
  analogWrite(yPin, y);
}

inline void ArduinoRenderer::dacWriteInline(uint32_t x, uint32_t y,
                                            uint32_t shift) {
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
inline void ArduinoRenderer::dacWriteDirect(uint32_t x, uint32_t y,
                                            uint32_t shift) {
  DAC->DATA[0].reg = x << shift;
  DAC->DATA[1].reg = y << shift;
}

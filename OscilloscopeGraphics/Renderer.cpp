#include "Renderer.h"

#define INPUT_BITS 16
#define DAC_MAX_RESOLUTION_BITS 12

using namespace osc;

void Renderer::setWriteMode(DACWriteMode mode) { writeMode = mode; }

void Renderer::drawPoint(Point<uint16_t>& point) {
  int32_t x = transform(point.x);
  int32_t y = transform(point.y);
  uint32_t shift = DAC_MAX_RESOLUTION_BITS - resolution;

  if (writeMode == DACWriteMode::INLINE) {
    dacWriteInline(x, y, shift);
  } else if (writeMode == DACWriteMode::DIRECT) {
    dacWriteDirect(x, y, shift);
  } else {
    dacWriteAnalogWrite(x, y);
  }
}

/*
 * Draw a line
 *
 * DDA line drawing algorithm implementation:
 * https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
 */
void Renderer::drawLine(Line<uint16_t>& line) {
  int32_t x0 = transform(line.a.x);
  int32_t y0 = transform(line.a.y);
  int32_t x1 = transform(line.b.x);
  int32_t y1 = transform(line.b.y);

  int32_t dx = x1 - x0;
  int32_t dy = y1 - y0;

  int32_t steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
  float ix = dx / (float)steps;
  float iy = dy / (float)steps;

  float x = x0;
  float y = y0;
  uint32_t shift = DAC_MAX_RESOLUTION_BITS - resolution;

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

// Scale input value to resolution value
inline uint32_t Renderer::transform(uint16_t value) {
  return value >> (INPUT_BITS - resolution);
}

inline void Renderer::dacWriteAnalogWrite(uint32_t x, uint32_t y) {
  analogWrite(xPin, x);
  analogWrite(yPin, y);
}

inline void Renderer::dacWriteInline(uint32_t x, uint32_t y, uint32_t shift) {
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

inline void Renderer::dacWriteDirect(uint32_t x, uint32_t y, uint32_t shift) {
  DAC->DATA[0].reg = x << shift;
  DAC->DATA[1].reg = y << shift;
}

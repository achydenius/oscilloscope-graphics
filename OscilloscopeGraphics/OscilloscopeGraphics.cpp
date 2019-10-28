#include "OscilloscopeGraphics.h"

void OscilloscopeGraphics::plot(uint32_t x, uint32_t y) {
  analogWrite(A0, x);
  analogWrite(A1, y);
}

void OscilloscopeGraphics::line(uint32_t x0, uint32_t y0, uint32_t x1,
                                uint32_t y1) {
  int32_t dx = x1 - x0;
  int32_t dy = y1 - y0;

  int32_t steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
  float ix = dx / (float)steps;
  float iy = dy / (float)steps;

  float x = x0;
  float y = y0;
  for (int32_t i = 0; i <= steps; i++) {
    analogWrite(A0, x);
    analogWrite(A1, y);
    x += ix;
    y += iy;
  }
}

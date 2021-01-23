#include "Renderer.h"

using namespace osc;

void Renderer::drawPoint(Point& point) {
  int32_t x = transform(point.x);
  int32_t y = transform(point.y);
  uint32_t shift = DAC_MAX_RESOLUTION_BITS - resolution;

  dacWrite(x, y, shift);
}

/*
 * Draw a line
 *
 * DDA line drawing algorithm implementation:
 * https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
 */
void Renderer::drawLine(Line& line) {
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

  for (int32_t i = 0; i < steps; i++) {
    dacWrite(x, y, shift);
    x += ix;
    y += iy;
  }
}

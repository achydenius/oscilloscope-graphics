#include "Renderer.h"

using namespace osc;

void Renderer::drawPoint(Point2D& point) {
  int32_t x = transform(point.x);
  int32_t y = transform(point.y);
  uint32_t shift = DAC_MAX_RESOLUTION_BITS - resolution;

  dacWrite(x, y, shift);
}

void Renderer::drawSolidLine(Line<Point2D>& line) {
  render(line, &Renderer::solidLoop);
}

void Renderer::drawDashedLine(Line<Point2D>& line) {
  render(line, &Renderer::dashedLoop);
}

/*
 * Draw a line
 *
 * DDA line drawing algorithm implementation:
 * https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
 */
// TODO: Use inheritance be used instead of passing the function as an argument?
void Renderer::render(Line<Point2D>& line,
                      void (Renderer::*loop)(int32_t, float, float, float,
                                             float, float, float, uint32_t)) {
  int32_t x0 = transform(line.a.x);
  int32_t y0 = transform(line.a.y);
  int32_t x1 = transform(line.b.x);
  int32_t y1 = transform(line.b.y);

  int32_t dx = x1 - x0;
  int32_t dy = y1 - y0;

  int32_t steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
  float length = sqrtf((dx * dx) + (dy * dy));
  float ix = dx / (float)steps;
  float iy = dy / (float)steps;
  float iu = length / (float)steps;

  float x = x0;
  float y = y0;
  float u = 0;
  uint32_t shift = DAC_MAX_RESOLUTION_BITS - resolution;

  (this->*loop)(steps, x, y, u, ix, iy, iu, shift);
}

void Renderer::solidLoop(int32_t steps, float x, float y, float u, float ix,
                         float iy, float iu, uint32_t shift) {
  for (int32_t i = 0; i < steps; i++) {
    dacWrite(x, y, shift);
    x += ix;
    y += iy;
  }
}

void Renderer::dashedLoop(int32_t steps, float x, float y, float u, float ix,
                          float iy, float iu, uint32_t shift) {
  int32_t i = 0;
  while (i <= steps) {
    // TODO: Parameterize these
    if (u < 32) {
      dacWrite(x, y, shift);
    }
    if (u >= 64) {
      u -= 64;
    }

    i++;
    x += ix;
    y += iy;
    u += iu;
  }
}

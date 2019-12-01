#include "Renderer.h"
#include <Arduino.h>

// Transform point from canvas space to output space
#define transform(p) ((uint32_t)(((p * 0.5) + 0.5) * maxValue))

using namespace osc;

void Renderer::setWriteMode(DACWriteMode mode) { writeMode = mode; }

void Renderer::setViewport(float top, float bottom, float left, float right) {
  viewport = {top, bottom, left, right};
}

void Renderer::drawPoint(float x, float y) {
  if (x < viewport.left || x > viewport.right || y < viewport.bottom ||
      y > viewport.top) {
    return;
  }

  analogWrite(xPin, transform(x));
  analogWrite(yPin, transform(y));
}

/*
 * Clip and draw a line
 *
 * Cohen-Sutherland line clipping algorithm implementation:
 * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 */
void Renderer::drawLine(float x0, float y0, float x1, float y1, bool clip) {
  uint32_t c0 = getClipCode(x0, y0);
  uint32_t c1 = getClipCode(x1, y1);
  bool accept = false;

  while (true) {
    if (!clip) {
      // No clipping - always accept
      accept = true;
      break;

    } else if (!(c0 | c1)) {
      // Both points inside window
      accept = true;
      break;

    } else if (c0 & c1) {
      // Both points outside window
      break;

    } else {
      // One or both points need to be clipped
      uint32_t c = c0 ? c0 : c1;
      float x, y;

      if (c & CLIP_TOP) {
        // Point is above the clip window
        x = x0 + (x1 - x0) * (viewport.top - y0) / (y1 - y0);
        y = viewport.top;
      } else if (c & CLIP_BOTTOM) {
        // Point is below the clip window
        x = x0 + (x1 - x0) * (viewport.bottom - y0) / (y1 - y0);
        y = viewport.bottom;
      } else if (c & CLIP_RIGHT) {
        // Point is to the right of clip window
        x = viewport.right;
        y = y0 + (y1 - y0) * (viewport.right - x0) / (x1 - x0);
      } else if (c & CLIP_LEFT) {
        // Point is to the left of clip window
        x = viewport.left;
        y = y0 + (y1 - y0) * (viewport.left - x0) / (x1 - x0);
      }

      if (c == c0) {
        x0 = x;
        y0 = y;
        c0 = getClipCode(x0, y0);
      } else {
        x1 = x;
        y1 = y;
        c1 = getClipCode(x1, y1);
      }
    }
  }

  if (accept) {
    outputLine(transform(x0), transform(y0), transform(x1), transform(y1));
  }
}

void Renderer::drawViewport() {
  // Draw viewport
  drawLine(-1.0, viewport.top, 1.0, viewport.top, false);
  drawLine(-1.0, viewport.bottom, 1.0, viewport.bottom, false);
  drawLine(viewport.left, 1.0, viewport.left, -1.0, false);
  drawLine(viewport.right, 1.0, viewport.right, -1.0, false);

  // Draw crosshair
  drawLine(-1.0, 1.0, 1.0, -1.0, false);
  drawLine(1.0, 1.0, -1.0, -1.0, false);
}

uint32_t Renderer::getClipCode(float x, float y) {
  uint32_t code = CLIP_INSIDE;

  if (x < viewport.left) {
    code |= CLIP_LEFT;
  } else if (x > viewport.right) {
    code |= CLIP_RIGHT;
  }
  if (y < viewport.bottom) {
    code |= CLIP_BOTTOM;
  } else if (y > viewport.top) {
    code |= CLIP_TOP;
  }

  return code;
}

/*
 * Output a line
 *
 * DDA line drawing algorithm implementation:
 * https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
 */
void Renderer::outputLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
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

#include "clipper.h"

using namespace osc;

Viewport* Clipper::getViewport() { return &viewport; }

void Clipper::setViewport(Viewport& vp) { viewport = vp; }

unsigned int Clipper::getClipCode(float x, float y) {
  unsigned int code = CLIP_INSIDE;

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
 * Clip line against set viewport
 *
 * Cohen-Sutherland line clipping algorithm implementation:
 * https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
 */
bool Clipper::clipLine(vec2& a, vec2& b) {
  char c0 = getClipCode(a[0], a[1]);
  char c1 = getClipCode(b[0], b[1]);
  bool accept = false;

  while (true) {
    if (!(c0 | c1)) {
      // Both points inside window
      accept = true;
      break;

    } else if (c0 & c1) {
      // Both points outside window
      break;

    } else {
      // One or both points need to be clipped
      char c = c0 ? c0 : c1;
      float x, y;

      if (c & CLIP_TOP) {
        // Point is above the clip window
        x = a[0] + (b[0] - a[0]) * (viewport.top - a[1]) / (b[1] - a[1]);
        y = viewport.top;
      } else if (c & CLIP_BOTTOM) {
        // Point is below the clip window
        x = a[0] + (b[0] - a[0]) * (viewport.bottom - a[1]) / (b[1] - a[1]);
        y = viewport.bottom;
      } else if (c & CLIP_RIGHT) {
        // Point is to the right of clip window
        x = viewport.right;
        y = a[1] + (b[1] - a[1]) * (viewport.right - a[0]) / (b[0] - a[0]);
      } else if (c & CLIP_LEFT) {
        // Point is to the left of clip window
        x = viewport.left;
        y = a[1] + (b[1] - a[1]) * (viewport.left - a[0]) / (b[0] - a[0]);
      }

      if (c == c0) {
        a[0] = x;
        a[1] = y;
        c0 = getClipCode(a[0], a[1]);
      } else {
        b[0] = x;
        b[1] = y;
        c1 = getClipCode(b[0], b[1]);
      }
    }
  }

  return accept;
}

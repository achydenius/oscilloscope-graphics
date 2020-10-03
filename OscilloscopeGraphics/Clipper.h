#ifndef __CLIPPER__
#define __CLIPPER__

#include "Renderer.h"

namespace osc {
struct Viewport {
  float top, bottom, left, right;
};

class Clipper {
  unsigned int CLIP_INSIDE = 0;
  unsigned int CLIP_LEFT = 1;
  unsigned int CLIP_RIGHT = 2;
  unsigned int CLIP_BOTTOM = 4;
  unsigned int CLIP_TOP = 8;
  Viewport viewport;

 public:
  Clipper() { viewport = {1.0, -1.0, -1.0, 1.0}; }
  Viewport* getViewport();
  void setViewport(Viewport& vp);
  bool clipLine(vec2& a, vec2& b);

 private:
  unsigned int getClipCode(float x, float y);
};
}  // namespace osc

#endif

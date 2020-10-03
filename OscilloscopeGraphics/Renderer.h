#ifndef __RENDERER__
#define __RENDERER__

#include "src/cglm/include/cglm/cglm.h"

namespace osc {
class Renderer {
 public:
  virtual void drawLine(vec2& a, vec2& b) = 0;
};
}  // namespace osc

#endif

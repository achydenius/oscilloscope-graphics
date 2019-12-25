#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"

namespace osc {
class Kaleidoscope : public Engine {
 public:
  enum Mode { HORIZONTAL, VERTICAL, QUAD };

 private:
  Mode mode;

 public:
  Kaleidoscope(int resolution, int xPin, int yPin, Mode mode = Mode::HORIZONTAL)
      : Engine(resolution, xPin, yPin), mode(mode){};
  void render(Object** objects, int objectCount, Camera& camera);

 private:
  void renderObjects(Object** objects, int objectCount, Camera& camera,
                     mat4* post = 0);
};
}  // namespace osc

#endif

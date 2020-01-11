#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {
class Kaleidoscope : public Engine {
 public:
  enum Mode { HORIZONTAL, VERTICAL, QUAD };

 private:
  const static int clipVertexCount = 4;
  vec3 clipVertices[clipVertexCount] = {
      {-0.5, 0.5, 0}, {0.5, 0.5, 0}, {0.5, -0.5, 0}, {-0.5, 0.5, 0}};
  vec3* clipNormals;
  Mode mode;

 public:
  Kaleidoscope(int resolution, int xPin, int yPin, int maxVertices,
               Mode mode = Mode::HORIZONTAL)
      : Engine(resolution, xPin, yPin, maxVertices), mode(mode) {
    clipNormals = new vec3[clipVertexCount];
    calculateClipNormals();
  };

  void render(Object** objects, int objectCount, Camera& camera);

 private:
  void clipLine(float x0, float y0, float x1, float y1);
  void calculateClipNormals();
};
}  // namespace osc

#endif

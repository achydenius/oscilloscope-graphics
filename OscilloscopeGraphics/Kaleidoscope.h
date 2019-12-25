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
  Kaleidoscope(int resolution, int xPin, int yPin, int maxVertices,
               Mode mode = Mode::HORIZONTAL)
      : Engine(resolution, xPin, yPin, maxVertices), mode(mode){};

  void render(Object** objects, int objectCount, Camera& camera) {
    if (mode == Mode::HORIZONTAL) {
      Renderer::Window windows[] = {{1, -1, 0, 1}, {1, -1, -1, 0}};
      mat4 matrices[] = {
          GLM_MAT4_IDENTITY_INIT,
          {{-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

      for (int i = 0; i < 2; i++) {
        renderer->setViewport(windows[i]);
        renderObjects(objects, objectCount, camera, &matrices[i]);
      }

    } else if (mode == Mode::VERTICAL) {
      Renderer::Window windows[] = {{1, 0, -1, 1}, {0, -1, -1, 1}};
      mat4 matrices[] = {
          GLM_MAT4_IDENTITY_INIT,
          {{1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

      for (int i = 0; i < 2; i++) {
        renderer->setViewport(windows[i]);
        renderObjects(objects, objectCount, camera, &matrices[i]);
      }

    } else if (mode == Mode::QUAD) {
      Renderer::Window windows[] = {
          {1, 0, 0, 1}, {0, -1, 0, 1}, {0, -1, -1, 0}, {1, 0, -1, 0}};
      mat4 matrices[] = {
          GLM_MAT4_IDENTITY_INIT,
          {{1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
          {{-1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
          {{-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

      for (int i = 0; i < 4; i++) {
        renderer->setViewport(windows[i]);
        renderObjects(objects, objectCount, camera, &matrices[i]);
      }
    }
  }
};
}  // namespace osc

#endif

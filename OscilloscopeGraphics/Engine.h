#ifndef __ENGINE__
#define __ENGINE__

#include "Matrix.h"
#include "Renderer.h"

namespace osc {
struct Mesh {
 public:
  int vertexCount;
  Vector3D* vertices;
};

class Object {
 public:
  Mesh* mesh;
  Matrix rotation, translation;

  Object(Mesh* m) : mesh(m){};
  void setRotation(float x, float y, float z) { rotation.rotation(x, y, z); }
  void setTranslation(float x, float y, float z) {
    translation.translation(x, y, z);
  }
};

class Engine {
  Renderer* renderer;
  Vector2D* projected;

 public:
  Engine(int resolution, int maxVertices);
  void render(Object& object, Matrix& camera, float dist);
};
}  // namespace osc

#endif

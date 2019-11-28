#ifndef __ENGINE__
#define __ENGINE__

// #define PROFILE

#include "Renderer.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {
struct Edge {
  int a, b;
};

struct Mesh {
 public:
  int vertexCount;
  int edgeCount;
  vec3* vertices;
  Edge* edges;
};

class Object {
 public:
  Mesh* mesh;
  vec3 rotation, translation;

  Object(Mesh* m) : mesh(m){};
  void setRotation(float x, float y, float z) {
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
  }
  void setTranslation(float x, float y, float z) {
    translation[0] = x;
    translation[1] = y;
    translation[2] = z;
  }
};

class Engine {
  int xPin, yPin;
  Renderer* renderer;
  vec2* projected;

 public:
  Engine(int resolution, int xPin, int yPin, int maxVertices);
  void render(Object& object);
  Renderer* getRenderer();
};
}  // namespace osc

#endif

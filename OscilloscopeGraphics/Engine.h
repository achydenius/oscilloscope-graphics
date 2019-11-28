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

class Camera {
 public:
  vec3 eye, center, up;

  Camera() : up({0, 1.0, 0}){};
  void setEye(float x, float y, float z) {
    eye[0] = x;
    eye[1] = y;
    eye[2] = z;
  }
  void setCenter(float x, float y, float z) {
    center[0] = x;
    center[1] = y;
    center[2] = z;
  }
};

class Engine {
  int xPin, yPin;
  Renderer* renderer;
  vec2* projected;

 public:
  Engine(int resolution, int xPin, int yPin, int maxVertices);
  void render(Object** objects, int objectCount, Camera& camera);
  Renderer* getRenderer();
};
}  // namespace osc

#endif

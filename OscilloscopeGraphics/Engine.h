#ifndef __ENGINE__
#define __ENGINE__

// #define PROFILE

#include "Mesh.h"
#include "Renderer.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {
class Object {
 public:
  Mesh* mesh;
  vec3 rotation, translation, scaling;
  vec2* transformed;
  bool visible;

  Object(Mesh* m) : mesh(m) {
    setScaling(1.0);
    transformed = new vec2[mesh->vertexCount];
  };
  ~Object() { delete transformed; }

  void setRotation(float x, float y, float z) { setVector(rotation, x, y, z); }
  void setTranslation(float x, float y, float z) {
    setVector(translation, x, y, z);
  }
  void setScaling(float x, float y, float z) { setVector(scaling, x, y, z); }
  void setScaling(float scale) { setVector(scaling, scale, scale, scale); }

 private:
  void setVector(vec3 vector, float x, float y, float z) {
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
  }
};

class Camera {
 public:
  vec3 eye, center, up;
  float fov, aspect, near, far;

  Camera()
      : up({0, 1.0, 0}), fov(GLM_PI_4f), aspect(1.0), near(0.01), far(100.0){};

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

 protected:
  Renderer* renderer;

 public:
  Engine(int resolution, int xPin, int yPin);
  ~Engine();
  virtual void render(Object** objects, int objectCount, Camera& camera);
  Renderer* getRenderer();

 protected:
  void transformObject(Object& object, mat4& camera, float near,
                       mat4* post = 0);
  void renderObject(Object& object);
};
}  // namespace osc

#endif

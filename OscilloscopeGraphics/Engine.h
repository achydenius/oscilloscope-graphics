#ifndef __ENGINE__
#define __ENGINE__

// #define PROFILE

#include "Clipper.h"
#include "Mesh.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {

class Renderer;

struct Line {
  vec2 a, b;
};

class Object {
 public:
  Mesh* mesh;
  vec3 rotation, translation, scaling;
  vec2* projected;
  bool isVisible;

  Object(Mesh* m) : mesh(m) {
    setScaling(1.0);
    projected = new vec2[m->vertexCount];
  };

  ~Object() { delete projected; }

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
  Renderer* renderer;
  Clipper clipper;
  ClipPolygon* viewport;
  vec2 blankingPoint = {1.0, 1.0};
  static const int defaultViewportVertexCount = 4;
  vec2 defaultViewportVertices[defaultViewportVertexCount] = {
      {-1.0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {-1.0, -0.75}};
  Line* lines;
  int lineCount;
  int maxLines;

 public:
  Engine(Renderer* renderer, int maxLines = 1000)
      : renderer(renderer), maxLines(maxLines) {
    viewport = new ClipPolygon(defaultViewportVertices, 4);
    lines = new Line[maxLines];
  };
  ~Engine();
  void setViewport(ClipPolygon* vp);
  void setBlankingPoint(float x, float y);
  void render(Object** objects, int objectCount, Camera& camera);
  void renderViewport();

 private:
  void transformObjects(Object** objects, int objectCount, Camera& camera);
  void clipObjects(Object** objects, int objectCount);
  void renderLines();
};
}  // namespace osc

#endif

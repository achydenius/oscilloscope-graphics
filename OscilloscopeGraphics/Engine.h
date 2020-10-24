#ifndef __ENGINE__
#define __ENGINE__

// #define PROFILE

#include <algorithm>
#include <initializer_list>

#include "Clipper.h"
#include "Mesh.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {

class Renderer;

struct Line {
  vec2 a, b;
};

template <typename T>
class Array {
 protected:
  T* data;
  size_t size;

 public:
  Array(size_t size) : size(size) { data = new T[size]; }
  Array(std::initializer_list<T> il) : Array(il.size()) {
    std::copy(il.begin(), il.end(), data);
  }

  T& operator[](int index) { return data[index]; }
  size_t getSize() { return size; }
};

template <typename T>
class Buffer : public Array<T> {
  int index;

 public:
  Buffer(size_t size) : Array<T>(size){};

  T& operator[](int index) { return Array<T>::data[index]; }
  int count() { return index; }
  void reset() { index = 0; }
  void add(T d) { Array<T>::data[index++] = d; }
};

class Object {
 public:
  Mesh* mesh;
  vec3 rotation, translation, scaling;
  vec2* projected;

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
  Renderer& renderer;
  Clipper clipper;
  ClipPolygon* viewport;

  vec2 blankingPoint = {1.0, 1.0};
  static const int defaultViewportVertexCount = 4;
  vec2 defaultViewportVertices[defaultViewportVertexCount] = {
      {-1.0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {-1.0, -0.75}};

  Buffer<Object*> transformedObjects;
  Buffer<Line> clippedLines;

 public:
  Engine(Renderer& renderer, int maxObjects = 1000, int maxLines = 1000)
      : renderer(renderer),
        transformedObjects(maxObjects),
        clippedLines(maxLines) {
    viewport = new ClipPolygon(defaultViewportVertices, 4);
  };
  ~Engine();
  Renderer& getRenderer();
  void setViewport(ClipPolygon* vp);
  void setBlankingPoint(float x, float y);
  void render(Array<Object*>& objects, Camera& camera);
  void renderViewport();

  Buffer<Object*>& transformObjects(Array<Object*>& objects, Camera& camera);
  Buffer<Line>& clipObjects(Buffer<Object*>& objects);
  void renderLines(Buffer<Line>& lines);
};
}  // namespace osc

#endif

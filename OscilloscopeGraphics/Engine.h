#ifndef __ENGINE__
#define __ENGINE__

// #define PROFILE

#include "Renderer.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {
struct Edge {
  int a, b;
};

class Mesh {
 public:
  int vertexCount;
  int edgeCount;
  vec3* vertices;
  Edge* edges;
  vec4 boundingSphere;

  void calculateBoundingSphere() {
    vec3 min = {0, 0, 0};
    vec3 max = {0, 0, 0};

    // Calculate axis-aligned bounding box
    for (int i = 0; i < vertexCount; i++) {
      min[0] = glm_min(min[0], vertices[i][0]);
      max[0] = glm_max(max[0], vertices[i][0]);
      min[1] = glm_min(min[1], vertices[i][1]);
      max[1] = glm_max(max[1], vertices[i][1]);
      min[2] = glm_min(min[2], vertices[i][2]);
      max[2] = glm_max(max[2], vertices[i][2]);
    }

    // Calculate center and radius of the sphere
    vec3 center;
    float r = 0;
    glm_vec3_center(min, max, center);
    for (int i = 0; i < vertexCount; i++) {
      r = glm_max(r, glm_vec3_distance(vertices[i], center));
    }

    glm_vec4(center, r, boundingSphere);
  }
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
  Renderer* renderer;
  vec2* projected;

 public:
  Engine(int resolution, int xPin, int yPin, int maxVertices);
  void render(Object** objects, int objectCount, Camera& camera);
  Renderer* getRenderer();
};
}  // namespace osc

#endif

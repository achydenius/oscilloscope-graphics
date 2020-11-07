#ifndef __CAMERA__
#define __CAMERA__

#include "Engine.h"
#include "src/cglm/include/cglm/cglm.h"

namespace osc {
class Camera {
 public:
  virtual mat4& getMatrix() = 0;
  // TODO: Fix passing the matrix
  virtual bool isVisible(Object& object, mat4& final) = 0;
};

class LookAtCamera : public Camera {
 public:
  vec3 eye, center, up;
  float fov, aspect, near, far;
  mat4 matrix;

  LookAtCamera()
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

  mat4& getMatrix() {
    mat4 projection, view;

    glm_perspective(fov, aspect, near, far, projection);
    glm_lookat(eye, center, up, view);
    glm_mat4_mul(projection, view, matrix);

    return matrix;
  }

  bool isVisible(Object& object, mat4& final) {
    // Cull object against near plane
    // TODO: Make this a bit cleaner?
    vec3 sphereCenter;
    glm_mat4_mulv3(final, object.mesh->boundingSphere, 1.0, sphereCenter);
    float scale = glm_max(glm_max(object.scaling[0], object.scaling[1]),
                          object.scaling[2]);

    return sphereCenter[2] - object.mesh->boundingSphere[3] * scale >= near;
  }
};
}  // namespace osc

#endif

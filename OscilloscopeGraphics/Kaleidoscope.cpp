#include "Kaleidoscope.h"

using namespace osc;

void Kaleidoscope::renderObjects(Object** objects, int objectCount,
                                 Camera& camera, mat4* post) {
  mat4 projection, view, matrix;

  // Get view & projection matrices
  glm_perspective(camera.fov, camera.aspect, camera.near, camera.far,
                  projection);
  glm_lookat(camera.eye, camera.center, camera.up, view);
  glm_mat4_mul(projection, view, matrix);

  for (int i = 0; i < objectCount; i++) {
    Object* object = objects[i];
    transformObject(*object, matrix, camera.near, post);
    if (object->visible) {
      renderObject(*object);
    }
  }
}

void Kaleidoscope::render(Object** objects, int objectCount, Camera& camera) {
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

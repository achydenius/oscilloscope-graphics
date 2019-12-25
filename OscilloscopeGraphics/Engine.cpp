#include "Engine.h"

#ifdef PROFILE
#include "Timer.h"
#define TIMER_START(timer) timer.start()
#define TIMER_STOP(timer) timer.stop()
#define TIMER_PRINT(timer, name) timer.print(name)
#else
#define TIMER_START(timer)
#define TIMER_STOP(timer)
#define TIMER_PRINT(timer, name)
#endif

using namespace osc;

Engine::Engine(int resolution, int xp, int yp) {
  xPin = xp;
  yPin = yp;
  renderer =
      new Renderer(resolution, xPin, yPin, Renderer::DACWriteMode::STANDARD);
}

Engine::~Engine() { delete renderer; }

void Engine::render(Object** objects, int objectCount, Camera& camera) {
  mat4 projection, view, matrix;

#ifdef PROFILE
  Timer transformTimer, renderTimer;
#endif

  // Get view & projection matrices
  glm_perspective(camera.fov, camera.aspect, camera.near, camera.far,
                  projection);
  glm_lookat(camera.eye, camera.center, camera.up, view);
  glm_mat4_mul(projection, view, matrix);

  for (int i = 0; i < objectCount; i++) {
    Object* object = objects[i];

    TIMER_START(transformTimer);
    transformObject(*object, matrix, camera.near);
    TIMER_STOP(transformTimer);

    TIMER_START(renderTimer);
    if (object->visible) {
      renderObject(*object);
    }
    TIMER_STOP(renderTimer);

    TIMER_PRINT(transformTimer, "transform");
    TIMER_PRINT(renderTimer, "render");
  }
}

void Engine::transformObject(Object& object, mat4& camera, float near,
                             mat4* post) {
  mat4 scaling, translation, rotation, matrix;

  // Calculate the final transformation matrix
  glm_scale_make(scaling, object.scaling);
  glm_translate_make(translation, object.translation);
  glm_euler(object.rotation, rotation);

  mat4* matrices[] = {&camera, &translation, &rotation, &scaling};
  glm_mat4_mulN(matrices, 4, matrix);

  // Cull object against near plane
  // TODO: Make this a bit cleaner?
  vec3 sphereCenter;
  glm_mat4_mulv3(matrix, object.mesh->boundingSphere, 1.0, sphereCenter);
  float scale =
      glm_max(glm_max(object.scaling[0], object.scaling[1]), object.scaling[2]);

  object.visible =
      sphereCenter[2] - object.mesh->boundingSphere[3] * scale >= near;
  if (!object.visible) {
    return;
  }

  // Apply post processing matrix if defined
  if (post) {
    glm_mat4_mul(*post, matrix, matrix);
  }

  for (int i = 0; i < object.mesh->vertexCount; i++) {
    vec4 vertex, transformed;
    glm_vec4(object.mesh->vertices[i], 1.0, vertex);
    glm_mat4_mulv(matrix, vertex, transformed);

    object.transformed[i][0] = transformed[0] / transformed[3];
    object.transformed[i][1] = transformed[1] / transformed[3];
  }
}

void Engine::renderObject(Object& object) {
  for (int i = 0; i < object.mesh->edgeCount; i++) {
    Edge* edge = &object.mesh->edges[i];
    renderer->clipAndDrawLine(
        object.transformed[edge->a][0], object.transformed[edge->a][1],
        object.transformed[edge->b][0], object.transformed[edge->b][1]);
  }
}

Renderer* Engine::getRenderer() { return renderer; }

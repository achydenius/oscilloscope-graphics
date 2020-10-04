#include "Engine.h"

#include "Mesh.h"

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

void Engine::render(Object** objects, int objectCount, Camera& camera) {
#ifdef PROFILE
  Timer transformTimer, renderTimer;
#endif

  TIMER_START(transformTimer);
  transformObjects(objects, objectCount, camera);
  TIMER_STOP(transformTimer);

  TIMER_START(renderTimer);
  renderObjects(objects, objectCount);
  TIMER_STOP(renderTimer);

  TIMER_PRINT(transformTimer, "transform");
  TIMER_PRINT(renderTimer, "render");
}

void Engine::transformObjects(Object** objects, int objectCount,
                              Camera& camera) {
  mat4 projection, view, scaling, translation, rotation, matrix;

  // Get view & projection matrices
  glm_perspective(camera.fov, camera.aspect, camera.near, camera.far,
                  projection);
  glm_lookat(camera.eye, camera.center, camera.up, view);

  for (int i = 0; i < objectCount; i++) {
    Object* object = objects[i];

    // Calculate the final transformation matrix
    glm_scale_make(scaling, object->scaling);
    glm_translate_make(translation, object->translation);
    glm_euler(object->rotation, rotation);

    mat4* matrices[] = {&projection, &view, &translation, &rotation, &scaling};
    glm_mat4_mulN(matrices, 5, matrix);

    // Cull object against near plane
    // TODO: Make this a bit cleaner?
    vec3 sphereCenter;
    glm_mat4_mulv3(matrix, object->mesh->boundingSphere, 1.0, sphereCenter);
    float scale = glm_max(glm_max(object->scaling[0], object->scaling[1]),
                          object->scaling[2]);

    if (sphereCenter[2] - object->mesh->boundingSphere[3] * scale >=
        camera.near) {
      object->isVisible = true;

      // Transform vertices
      for (int i = 0; i < object->mesh->vertexCount; i++) {
        vec4 vertex, transformed;
        glm_vec4(object->mesh->vertices[i], 1.0, vertex);
        glm_mat4_mulv(matrix, vertex, transformed);

        object->projected[i][0] = transformed[0] / transformed[3];
        object->projected[i][1] = transformed[1] / transformed[3];
      }
    } else {
      object->isVisible = false;
    }
  }
}

void Engine::renderObjects(Object** objects, int objectCount) {
  for (int i = 0; i < objectCount; i++) {
    Object* object = objects[i];

    if (object->isVisible) {
      for (int j = 0; j < object->mesh->edgeCount; j++) {
        Edge* edge = &object->mesh->edges[j];
        vec2 a = {object->projected[edge->a][0], object->projected[edge->a][1]};
        vec2 b = {object->projected[edge->b][0], object->projected[edge->b][1]};

        if (clipper.clipLine(a, b)) {
          renderer->drawLine(a, b);
        }
      }
    }
  }
}

void Engine::renderViewport() {
  Viewport* viewport = clipper.getViewport();

  vec2 lines[][2] = {// Viewport
                     {-1.0, viewport->top, 1.0, viewport->top},
                     {-1.0, viewport->bottom, 1.0, viewport->bottom},
                     {viewport->left, 1.0, viewport->left, -1.0},
                     {viewport->right, 1.0, viewport->right, -1.0},
                     // Crosshair
                     {-1.0, 1.0, 1.0, -1.0},
                     {1.0, 1.0, -1.0, -1.0}};

  for (int i = 0; i < 6; i++) {
    renderer->drawLine(lines[i][0], lines[i][1]);
  }
}

Renderer* Engine::getRenderer() { return renderer; }

Clipper* Engine::getClipper() { return &clipper; }

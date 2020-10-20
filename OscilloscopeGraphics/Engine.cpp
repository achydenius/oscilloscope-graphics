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

Engine::~Engine() { delete viewport; }

Renderer& Engine::getRenderer() { return renderer; }

void Engine::setViewport(ClipPolygon* vp) { viewport = vp; }

void Engine::setBlankingPoint(float x, float y) {
  blankingPoint[0] = x;
  blankingPoint[1] = y;
}

void Engine::render(Array<Object*>& objects, Camera& camera) {
#ifdef PROFILE
  Timer transformTimer, clipTimer, renderTimer;
#endif

  TIMER_START(transformTimer);
  Buffer<Object*>& transformed = transformObjects(objects, camera);
  TIMER_STOP(transformTimer);

  TIMER_START(clipTimer);
  Buffer<Line>& clipped = clipObjects(transformed);
  TIMER_STOP(clipTimer);

  TIMER_START(renderTimer);
  renderLines(clipped);
  TIMER_STOP(renderTimer);

  // Move oscilloscope beam to blanking point (i.e. outside screen) when
  // finished drawing
  renderer.drawPoint(blankingPoint);

  TIMER_PRINT(transformTimer, "transform");
  TIMER_PRINT(renderTimer, "render");
}

Buffer<Object*>& Engine::transformObjects(Array<Object*>& objects,
                                          Camera& camera) {
  mat4 projection, view, scaling, translation, rotation, matrix;

  // Get view & projection matrices
  glm_perspective(camera.fov, camera.aspect, camera.near, camera.far,
                  projection);
  glm_lookat(camera.eye, camera.center, camera.up, view);

  transformedObjects.reset();
  for (int i = 0; i < objects.getSize(); i++) {
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
      // Transform vertices
      for (int i = 0; i < object->mesh->vertexCount; i++) {
        vec4 vertex, transformed;
        glm_vec4(object->mesh->vertices[i], 1.0, vertex);
        glm_mat4_mulv(matrix, vertex, transformed);

        object->projected[i][0] = transformed[0] / transformed[3];
        object->projected[i][1] = transformed[1] / transformed[3];
      }
      transformedObjects.add(object);
    }
  }
  return transformedObjects;
}

Buffer<Line>& Engine::clipObjects(Buffer<Object*>& objects) {
  clippedLines.reset();
  for (int i = 0; i < objects.count(); i++) {
    Object* object = objects[i];

    for (int j = 0; j < object->mesh->edgeCount; j++) {
      Edge* edge = &object->mesh->edges[j];
      vec2 a = {object->projected[edge->a][0], object->projected[edge->a][1]};
      vec2 b = {object->projected[edge->b][0], object->projected[edge->b][1]};

      if (clipper.clipLine(a, b, *viewport)) {
        Line line;
        glm_vec2_copy(a, line.a);
        glm_vec2_copy(b, line.b);
        clippedLines.add(line);
      }
    }
  }
  return clippedLines;
}

void Engine::renderLines(Buffer<Line>& lines) {
  for (int i = 0; i < lines.count(); i++) {
    renderer.drawLine(lines[i].a, lines[i].b);
  }
}

void Engine::renderViewport() {
  for (int i = 0; i < viewport->vertexCount; i++) {
    renderer.drawLine(viewport->vertices[i],
                      viewport->vertices[(i + 1) % viewport->vertexCount]);
  }
}

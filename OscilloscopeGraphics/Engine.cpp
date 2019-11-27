#include "Engine.h"
#ifdef PROFILE
#include "Timer.h"
#endif

using namespace osc;

Engine::Engine(int resolution, int xp, int yp, int maxVertices) {
  xPin = xp;
  yPin = yp;
  renderer = new Renderer(resolution, xPin, yPin);
  projected = new vec2[maxVertices];
}

void Engine::render(Object& object) {
  mat4 projection, translation, rotation, matrix;

#ifdef PROFILE
  Timer timer;
  timer.start();
#endif

  glm_perspective_default(1.0, projection);
  glm_translate_make(translation, object.translation);
  glm_euler(object.rotation, rotation);

  mat4* matrices[] = {&projection, &translation, &rotation};
  glm_mat4_mulN(matrices, 3, matrix);

  for (int i = 0; i < object.mesh->vertexCount; i++) {
    vec4 vertex, transformed;
    glm_vec4(object.mesh->vertices[i], 1.0, vertex);
    glm_mat4_mulv(matrix, vertex, transformed);

    projected[i][0] = transformed[0] / transformed[3];
    projected[i][1] = transformed[1] / transformed[3];
  }

#ifdef PROFILE
  timer.stop();
  timer.print("transform");
  timer.start();
#endif

  for (int i = 0; i < object.mesh->edgeCount; i++) {
    Edge* edge = &object.mesh->edges[i];
    renderer->line(projected[edge->a][0], projected[edge->a][1],
                   projected[edge->b][0], projected[edge->b][1]);
  }

#ifdef PROFILE
  timer.stop();
  timer.print("render");
#endif
}

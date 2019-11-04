#include "Engine.h"

using namespace osc;

Engine::Engine(int resolution, int xp, int yp, int maxVertices) {
  xPin = xp;
  yPin = yp;
  renderer = new Renderer(resolution, xPin, yPin);
  projected = new Vector2D[maxVertices];
}

void Engine::render(Object& object, Matrix& camera, float dist) {
  Matrix world, view;
  world.multiply(object.translation, object.rotation);
  view.multiply(camera, world);

  for (int i = 0; i < object.mesh->vertexCount; i++) {
    Vector3D transformed;
    view.multiply(object.mesh->vertices[i], transformed);
    transformed.project(projected[i], dist);
  }

  for (int i = 0; i < object.mesh->edgeCount; i++) {
    Edge* edge = &object.mesh->edges[i];
    osc::Vector2D a = projected[edge->a];
    osc::Vector2D b = projected[edge->b];
    renderer->line(a.x, a.y, b.x, b.y);
  }
}

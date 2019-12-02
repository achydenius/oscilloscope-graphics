#include <OscilloscopeGraphics.h>

osc::Engine engine(10, A0, A1, 8);

vec3 vertices[] = {
  { -0.5, 0.5, 0.5 },
  { -0.5, 0.5, -0.5 },
  { 0.5, 0.5, -0.5 },
  { 0.5, 0.5, 0.5 },
  { -0.5, -0.5, 0.5 },
  { -0.5, -0.5, -0.5 },
  { 0.5, -0.5, -0.5 },
  { 0.5, -0.5, 0.5 }
};
osc::Edge edges[] = {
  { 0, 1 },
  { 1, 2 },
  { 2, 3 },
  { 3, 0 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },
  { 7, 4 },
  { 0, 4 },
  { 1, 5 },
  { 2, 6 },
  { 3, 7 }
};
osc::Mesh mesh = {
  8, 12, vertices, edges
};
int objectCount = 8;
osc::Object **objects;
osc::Camera camera;

void setup() {
  engine.getRenderer()->setWriteMode(osc::Renderer::DACWriteMode::INLINE);
  engine.getRenderer()->setViewport(0.75, -0.75, -1.0, 1.0);

  mesh.calculateBoundingSphere();

  objects = new osc::Object*[objectCount];
  for (int i = 0; i < objectCount; i++) {
    objects[i] = new osc::Object(&mesh);
  }
  camera.setCenter(0, 0, 0);
}

float phase = 0;
void loop() {
  for (int i = 0; i < objectCount; i += 4) {
    float y = i == 0 ? 1 : -1;
    objects[i]->setTranslation(-1, sin(phase) * 2.0 + y, 1);
    objects[i + 1]->setTranslation(1, sin(phase + 0.5) * 2.0 + y, 1);
    objects[i + 2]->setTranslation(-1, sin(phase + 1.0) * 2.0 + y, -1);
    objects[i + 3]->setTranslation(1, sin(phase + 1.5) * 2.0 + y, -1);  
  }

  for (int i = 0; i < objectCount; i++) {
    objects[i]->setRotation(0, phase, 0);
  }

  camera.setEye(sin(phase) * 5.0, 0, cos(phase * 0.25) * 5.0);
  
  engine.render(objects, objectCount, camera);

  // engine.getRenderer()->drawViewport();

  phase += 0.01;
}

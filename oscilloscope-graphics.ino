#include <OscilloscopeGraphics.h>

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

osc::Engine engine(10, A0, A1, 8);

int objectCount = 8;
osc::Mesh *mesh = osc::MeshBuilder::createCube(1.0);
osc::Object **objects;
osc::Camera camera;

void setup() {
  engine.getRenderer()->setWriteMode(osc::Renderer::DACWriteMode::INLINE);
  osc::Renderer::Window viewport = { 0.75, -0.75, -1.0, 1.0 };
  engine.getRenderer()->setViewport(viewport);

  objects = new osc::Object*[objectCount];
  for (int i = 0; i < objectCount; i++) {
    objects[i] = new osc::Object(mesh);
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
    objects[i]->setScaling(map_float(sin(phase), -1.0, 1.0, 0.5, 1.0));
  }

  camera.setEye(sin(phase) * 5.0, 0, cos(phase * 0.25) * 5.0);

  engine.render(objects, objectCount, camera);

  // engine.getRenderer()->drawViewport();

  phase += 0.01;
}

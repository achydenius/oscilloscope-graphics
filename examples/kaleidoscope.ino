#include <Kaleidoscope.h>
#include <OscilloscopeGraphics.h>

osc::Engine *engine;

osc::Mesh *mesh = osc::MeshBuilder::createCube(1.0);
int objectCount = 2;
osc::Object **objects;
osc::Camera camera;

void setup() {
  engine = new osc::Kaleidoscope(10, A0, A1, 8, osc::Kaleidoscope::QUAD);
  engine->getRenderer()->setWriteMode(osc::Renderer::DACWriteMode::INLINE);

  objects = new osc::Object *[objectCount];
  for (int i = 0; i < objectCount; i++) {
    objects[i] = new osc::Object(mesh);
  }

  camera.setEye(0, 0, 4.0);
  camera.setCenter(0, 0, 0);
}

float phase = 0;
void loop() {
  for (int i = 0; i < objectCount; i++) {
    objects[i]->setRotation(phase * 0.03, phase, phase * 0.1);
    objects[i]->setTranslation(sin(phase + i), 0, 0);
    objects[i]->setScaling(sin(phase + i * 4.0) * 0.25 + 0.75);
  }

  engine->render(objects, objectCount, camera);

  phase += 0.01;
}

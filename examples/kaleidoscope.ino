#include <Kaleidoscope.h>
#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1,
                              osc::ArduinoRenderer::DACWriteMode::INLINE);
osc::Kaleidoscope engine(renderer, osc::Kaleidoscope::Mode::FOUR);

int objectCount = 2;
osc::Mesh *mesh = osc::MeshBuilder::createCube(1.0);
osc::Array<osc::Object *> objects(objectCount);
osc::Camera camera;

void setup() {
  for (int i = 0; i < objects.getSize(); i++) {
    objects[i] = new osc::Object(mesh);
  }

  camera.setEye(0, 0, 4.0);
  camera.setCenter(0, 0, 0);
}

float phase = 0;
void loop() {
  for (int i = 0; i < objects.getSize(); i++) {
    objects[i]->setRotation(phase * 0.03, phase, phase * 0.1);
    objects[i]->setTranslation(sin(phase + i), 0, 0);
    objects[i]->setScaling(sin(phase + i * 4.0) * 0.25 + 0.75);
  }

  engine.render(objects, camera);

  phase += 0.01;
}

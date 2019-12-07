#include <OscilloscopeGraphics.h>

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

osc::Engine engine(10, A0, A1, 8);

int objectCount = 12;
osc::Mesh *mesh = osc::MeshBuilder::createCube(1.0);
osc::Object **objects;
osc::Camera camera;

void setup() {
  engine.getRenderer()->setWriteMode(osc::Renderer::DACWriteMode::INLINE);
  engine.getRenderer()->setViewport(0.75, -0.75, -1.0, 1.0);

  objects = new osc::Object*[objectCount];
  for (int i = 0; i < objectCount; i++) {
    objects[i] = new osc::Object(mesh);
  }
  camera.setCenter(0, 0, 0);
}

float phase = 0;
void loop() {
  long start = millis();
  
  for (int i = 0, j = 0; i < objectCount; i += 4, j++) {
    float y = (j * 3) - (objectCount / 4) + 0.5;
    objects[i]->setTranslation(-1 + cos(phase) * 0.5, sin(phase * 0.75) * 3.0 + y, 1 + cos(phase) * 0.5);
    objects[i + 1]->setTranslation(1 + cos(phase + 0.5) * 0.5, sin(phase * 0.75 + 0.5) * 3.0 + y, 1 + cos(phase + 0.5) * 0.5);
    objects[i + 2]->setTranslation(-1 + cos(phase + 1.0) * 0.5, sin(phase * 0.75 + 1.0) * 3.0 + y, -1 + cos(phase + 1.0) * 0.5);
    objects[i + 3]->setTranslation(1 + cos(phase + 1.5) * 0.5, sin(phase * 0.75 + 1.5) * 3.0 + y, -1 + cos(phase + 1.5) * 0.5);

    /* objects[i]->setScaling(map_float(sin(phase), -1.0, 1.0, 0.5, 1.0));
    objects[i + 1]->setScaling(map_float(sin(phase + 0.5), -1.0, 1.0, 0.5, 1.0));
    objects[i + 2]->setScaling(map_float(sin(phase + 1.0), -1.0, 1.0, 0.5, 1.0));
    objects[i + 3]->setScaling(map_float(sin(phase + 1.5), -1.0, 1.0, 0.5, 1.0)); */
  }

  for (int i = 0; i < objectCount; i++) {
    objects[i]->setRotation(0, phase, 0);
  }

  camera.setEye(sin(phase) * 5.0, 0, cos(phase * 0.25) * 5.0);
  // camera.setEye(0, 0, 10.0);

  engine.render(objects, objectCount, camera);

  long elapsed = millis() - start;

  /* if (elapsed < 15) {
    delay(15 - elapsed);
  } */

  // engine.getRenderer()->drawViewport();

  phase += 0.01;
}

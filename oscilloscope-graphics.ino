#include <math.h>
#include <OscilloscopeGraphics.h>

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

osc::Engine engine(10, 8);
osc::Vector3D vertices[] = {
  { -10.0, 10.0, 10.0 },
  { -10.0, 10.0, -10.0 },
  { 10.0, 10.0, -10.0 },
  { 10.0, 10.0, 10.0 },
  { -10.0, -10.0, 10.0 },
  { -10.0, -10.0, -10.0 },
  { 10.0, -10.0, -10.0 },
  { 10.0, -10.0, 10.0 }
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
  8,
  12,
  vertices,
  edges
};
osc::Object object(&mesh);
osc::Matrix camera;

void setup() {
}

float phase = 0;
void loop() {
  osc::Vector3D eye = { 0, sin(phase) * 5.0, -20.0 };
  osc::Vector3D target = { 0, 0, 0 };
  camera.lookAt(eye, target);

  object.setRotation(0, phase * 0.25, phase);
  object.setTranslation(sin(phase * 0.85) * 15.0, 0, 0);

  engine.render(object, camera, 10.0);

  phase += 0.01;
}

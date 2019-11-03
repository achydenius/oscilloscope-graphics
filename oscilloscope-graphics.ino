#include <math.h>
#include <OscilloscopeGraphics.h>

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

osc::Engine engine(10, 4);
osc::Vector3D vertices[4] = {
  { -10.0,  10.0, 0 },
  {  10.0,  10.0, 0 },
  {  10.0, -10.0, 0 },
  { -10.0, -10.0, 0 }
};
osc::Mesh mesh = {
  4,
  vertices
};
osc::Object object(&mesh);
osc::Matrix camera;

void setup() {
}

float phase = 0;
void loop() {
  osc::Vector3D eye = { 0, sin(phase) * 5.0, -10.0 };
  osc::Vector3D target = { 0, 0, 0 };
  camera.lookAt(eye, target);

  object.setRotation(0, 0, phase);
  object.setTranslation(sin(phase * 0.85) * 10.0, 0, 0);

  engine.render(object, camera, 5.0);

  phase += 0.01;
}

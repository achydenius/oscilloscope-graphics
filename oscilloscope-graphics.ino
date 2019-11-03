#include <math.h>
#include <OscilloscopeGraphics.h>

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

osc::Renderer renderer = osc::Renderer(10);

const int vertexCount = 4;
osc::Vector3D original[vertexCount] = {
  { -10.0,  10.0, 0 },
  {  10.0,  10.0, 0 },
  {  10.0, -10.0, 0 },
  { -10.0, -10.0, 0 }
};
osc::Vector3D transformed[vertexCount];
osc::Vector2D projected[vertexCount];
osc::Matrix rotation, translation, scaling, matrix;

void setup() {}

float phase = 0;
void loop() {
  rotation.rotation(0, phase * 0.3, phase);
  translation.translation(sin(phase) * 10.0, 0, 10.0);
  matrix.multiply(translation, rotation);
  
  for (int i = 0; i < vertexCount; i++) {
    matrix.transform(original[i], transformed[i]);
    transformed[i].project(projected[i], 5.0);
  }

  for (int i = 0; i < vertexCount; i++) {
    osc::Vector2D a = projected[i];
    osc::Vector2D b = projected[(i + 1) % 4];
    renderer.line(a.x, a.y, b.x, b.y);
  }
  
  phase += 0.01;
}

#include <math.h>
#include <OscilloscopeGraphics.h>

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

OscilloscopeGraphics gfx = OscilloscopeGraphics(10);

const int vertexCount = 4;
Vector3D original[vertexCount] = {
  { -1.0, 1.0, 0 },
  { 1.0, 1.0, 0 },
  { 1.0, -1.0, 0 },
  { -1.0, -1.0, 0 }
};
Vector3D transformed[vertexCount];
Vector2D projected[vertexCount];
Matrix rotation, scaling, matrix;

void setup() {}

float phase = 0;
void loop() {
  rotation.rotation(0, phase * 0.3, phase);
  scaling.scaling(mapf(sin(phase), -1.0, 1.0, 5.0, 7.5));
  matrix.multiply(rotation, scaling);
  
  for (int i = 0; i < vertexCount; i++) {
    matrix.transform(original[i], transformed[i]);
    transformed[i].project(projected[i], 10.0);
  }

  for (int i = 0; i < vertexCount; i++) {
    Vector2D a = projected[i];
    Vector2D b = projected[(i + 1) % 4];
    gfx.line(a.x, a.y, b.x, b.y);
  }
  
  phase += 0.01;
}

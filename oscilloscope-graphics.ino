#include <math.h>
#include <OscilloscopeGraphics.h>

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

OscilloscopeGraphics gfx = OscilloscopeGraphics(10);
Vector3D original[4] = {
  { -1.0, 1.0, 0 },
  { 1.0, 1.0, 0 },
  { 1.0, -1.0, 0 },
  { -1.0, -1.0, 0 }
};
Vector3D transformed[4];
Matrix rotation, scaling, matrix;

void setup() {}

float phase = 0;
void loop() {
  rotation.rotation(0, 0, phase);
  scaling.scaling(mapf(sin(phase), -1.0, 1.0, 0.25, 0.95));
  matrix.multiply(rotation, scaling);
  
  for (int i = 0; i < 4; i++) {
    matrix.transform(original[i], transformed[i]);
  }

  for (int i = 0; i < 4; i++) {
    Vector3D a = transformed[i];
    Vector3D b = transformed[(i + 1) % 4];
    gfx.line(a.x, a.y, b.x, b.y);
  }
  
  phase += 0.01;
}

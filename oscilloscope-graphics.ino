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
osc::Object object(&mesh); 

void setup() {}

float phase = 0;
void loop() {
  object.setRotation(0, phase * 0.5, phase);
  object.setTranslation(sin(phase), 0, -2.5);
  
  engine.render(object);

  phase += 0.01;
}

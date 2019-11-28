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
osc::Object objects[] = {
  &mesh, &mesh, &mesh, &mesh
};

void setup() {
  engine.getRenderer()->setWriteMode(osc::Renderer::DACWriteMode::INLINE);
}

float phase = 0;
void loop() {
  objects[0].setTranslation(-1, sin(phase), 1 - 5);
  objects[1].setTranslation(1, sin(phase + 0.5), 1 - 5);
  objects[2].setTranslation(-1, sin(phase + 1.0), -1 - 5);
  objects[3].setTranslation(1, sin(phase + 1.5), -1 - 5);

  for (int i = 0; i < 4; i++) {
    objects[i].setRotation(0, phase, 0);
  }
  
  engine.render(objects, 4);

  phase += 0.01;
}

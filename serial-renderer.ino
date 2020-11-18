#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1,
                              osc::ArduinoRenderer::DACWriteMode::INLINE);
osc::Buffer<osc::Line>* lines;
osc::Server server(115200);

void setup() { server.start(); }

void loop() {
  lines = server.update();

  for (int i = 0; i < lines->count(); i++) {
    renderer.drawLine((*lines)[i].a, (*lines)[i].b);
  }
}

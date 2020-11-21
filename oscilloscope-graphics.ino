#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1,
                              osc::ArduinoRenderer::DACWriteMode::INLINE);
osc::Server server(115200);
osc::Clipper clipper;
osc::OscilloscopeGraphics gfx(renderer, server, clipper);

void setup() { gfx.getServer().start(); }

void loop() { gfx.getAndRenderLines(); }

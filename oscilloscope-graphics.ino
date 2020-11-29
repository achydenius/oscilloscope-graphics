#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1,
                              osc::ArduinoRenderer::DACWriteMode::INLINE);
osc::Consumer consumer(115200);
osc::Clipper clipper;
osc::OscilloscopeGraphics gfx(renderer, consumer, clipper);

void setup() { gfx.getConsumer().start(); }

void loop() { gfx.getAndRenderLines(); }

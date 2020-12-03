#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1,
                              osc::ArduinoRenderer::DACWriteMode::INLINE);
osc::Consumer consumer(115200);
osc::OscilloscopeGraphics gfx(renderer, consumer);

void setup() { gfx.getConsumer().start(); }

void loop() { gfx.getAndRenderLines(); }

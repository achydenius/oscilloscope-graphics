#include <OscilloscopeGraphics.h>

osc::Renderer renderer(10, A0, A1, osc::Renderer::DACWriteMode::INLINE);
osc::UInt16Consumer consumer(115200);
osc::OscilloscopeGraphics gfx(renderer, consumer);

void setup() { gfx.getConsumer().start(); }

void loop() { gfx.getAndRenderLines(); }

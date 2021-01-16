#include <OscilloscopeGraphics.h>

osc::InlineRenderer renderer(10, A21, A22);
osc::UInt16Consumer consumer(115200);
osc::OscilloscopeGraphics gfx(renderer, consumer);

void setup() { gfx.getConsumer().start(); }

void loop() { gfx.getAndRenderLines(); }

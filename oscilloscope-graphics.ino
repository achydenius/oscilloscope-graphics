#include <OscilloscopeGraphics.h>

osc::InlineRenderer renderer(10, A21, A22);
osc::DefaultGraphics gfx(renderer);

void setup() { gfx.start(); }

void loop() { gfx.getAndRenderLines(); }

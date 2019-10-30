#include <OscilloscopeGraphics.h>

OscilloscopeGraphics gfx = OscilloscopeGraphics(8);

void setup() {}

void loop() {
  gfx.line(-0.5, -0.5, 0.5, -0.5);
  gfx.line(-0.5, 0.5, 0.5, 0.5);
  gfx.line(-0.75, 0, 0.75, 0);
  gfx.line(-1.25, -1.25, 1.25, 1.25);
}

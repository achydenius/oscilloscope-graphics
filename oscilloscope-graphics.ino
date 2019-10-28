#include <OscilloscopeGraphics.h>

OscilloscopeGraphics gfx = OscilloscopeGraphics(8);

void setup() {}

void loop() {
  gfx.line(0, 0, 255, 0);
  gfx.line(255, 0, 255, 255);
  gfx.line(255, 255, 0, 255);
  gfx.line(0, 255, 0, 0);
  gfx.line(0, 0, 255, 255);
  gfx.line(255, 0, 0, 255);
}

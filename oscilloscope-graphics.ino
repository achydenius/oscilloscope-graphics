#include <OscilloscopeGraphics.h>

OscilloscopeGraphics gfx = OscilloscopeGraphics(8);

void setup() {}

void loop() {
  gfx.clipLine(50, 50, 205, 50);
  gfx.clipLine(205, 50, 205, 205);
  gfx.clipLine(205, 205, 50, 205);
  gfx.clipLine(50, 205, 50, 50);

  gfx.clipLine(-50, -50, 305, -50);
  gfx.clipLine(-50, 305, 305, 305);

  gfx.clipLine(-50, 127, 100, 127);
  gfx.clipLine(305, 127, 155, 127);

  gfx.clipLine(-50, -50, 305, 305);
  gfx.clipLine(305, -50, -50, 305);
}

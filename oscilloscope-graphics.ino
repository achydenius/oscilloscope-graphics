#include <OscilloscopeGraphics.h>

OscilloscopeGraphics gfx = OscilloscopeGraphics(8);

void setup() {}

void loop() {
  gfx.plot(0, 0);
  delay(1);
  gfx.plot(1.0, 0);
  delay(1);
  gfx.plot(1.0, 1.0);
  delay(1);
  gfx.plot(0, 1.0);
  delay(1);
}

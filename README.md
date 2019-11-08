# Oscilloscope Graphics for Arduino

Simple vector graphics engine for Arduino and oscilloscope. Tested with [Adafruit Metro M4](https://www.adafruit.com/product/3382) but any Arduino compatible board with two DACs will most likely work.

The engine includes a tiny 3D math library, a renderer for drawing lines on oscilloscope and some basic primitives for creating objects.

![Picture of oscilloscope running example animation](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/square.jpg)

## Code example

Drawing and animating a square (as seen in the picture above):

```cpp
#include <math.h>
#include <OscilloscopeGraphics.h>

osc::Engine engine(10, A0, A1, 4);

osc::Vector3D vertices[] = {
  { -10.0, 10.0,  0 },
  {  10.0, 10.0,  0 },
  {  10.0, -10.0, 0 },
  { -10.0, -10.0, 0 }
};
osc::Edge edges[] = {
  { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }
};
osc::Mesh mesh = {
  4, 4, vertices, edges
};
osc::Object object(&mesh);

osc::Matrix camera;

void setup() {
  camera.translation(0, 0, 20.0);
}

float phase = 0;
void loop() {
  object.setRotation(0, 0, phase);
  object.setTranslation(sin(phase * 0.85) * 15.0, 0, 0);

  engine.render(object, camera, 10.0);

  phase += 0.01;
}
```

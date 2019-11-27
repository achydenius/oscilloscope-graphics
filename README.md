# Oscilloscope Graphics for Arduino

Simple vector graphics engine for Arduino and oscilloscope. Tested with [Adafruit Metro M4](https://www.adafruit.com/product/3382) but any Arduino compatible board with two DACs will most likely work.

![Picture of oscilloscope running example animation](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/square.jpg)

## Installation

1. Get [cglm library](https://github.com/recp/cglm) and move it inside _src_ folder
2. Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder

## Code example

Drawing and animating a square (as seen in the picture above):

```cpp
#include <OscilloscopeGraphics.h>

osc::Engine engine(10, A0, A1, 4);

vec3 vertices[] = {
  { -1.0, 1.0,  0 },
  {  1.0, 1.0,  0 },
  {  1.0, -1.0, 0 },
  { -1.0, -1.0, 0 }
};
osc::Edge edges[] = {
  { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }
};
osc::Mesh mesh = {
  4, 4, vertices, edges
};
osc::Object object(&mesh);

void setup() {}

float phase = 0;
void loop() {
  object.setRotation(0, 0, phase);
  object.setTranslation(sin(phase), 0, -4.0);

  engine.render(object);

  phase += 0.01;
}
```

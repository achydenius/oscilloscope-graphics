# Oscilloscope Graphics for Arduino

Simple vector graphics engine for Arduino and oscilloscope. Tested with [Adafruit Metro M4](https://www.adafruit.com/product/3382) but any Arduino compatible board with two DACs will most likely work.

![Picture of oscilloscope running an example](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/media-lab-demo-day.jpg)

## Installation

1. Get [cglm library](https://github.com/recp/cglm) and move it inside _src_ folder
2. Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder

## Code example

Drawing and animating a simple square:

```cpp
#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1);
osc::Engine engine(renderer);

osc::Mesh *mesh = osc::MeshBuilder::createPlane(1.0);
osc::Array<osc::Object*> objects({ new osc::Object(mesh) });
osc::LookAtCamera camera;

void setup() {
  camera.setEye(0, 0, 4.0);
  camera.setCenter(0, 0, 0);
}

float phase = 0;
void loop() {
  objects[0]->setRotation(M_PI_2, phase, 0);
  objects[0]->setTranslation(sin(phase), 0, 0);
  objects[0]->setScaling(sin(phase * 4.0) * 0.25 + 0.75);

  engine.render(objects, camera);

  phase += 0.002;
}
```

More examples can be found in [examples/](examples/) directory.

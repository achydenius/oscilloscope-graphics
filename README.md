# Oscilloscope Graphics for Arduino

Simple vector graphics engine for Arduino and oscilloscope. Tested with [Adafruit Metro M4](https://www.adafruit.com/product/3382) but any Arduino compatible board with two DACs will most likely work.

![Picture of oscilloscope running an example](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/media-lab-demo-day.jpg)

## Library installation

1. Get [cglm library](https://github.com/recp/cglm) and move it inside _src_ folder
2. Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder

## Local development with emulator

1. Install [SDL](https://www.libsdl.org/) with `brew install sdl2`
2. Build and run with `make && ./main`

## Code example

Drawing and animating a simple square:

```cpp
#include <OscilloscopeGraphics.h>

osc::ArduinoRenderer renderer(10, A0, A1);
osc::Engine engine(&renderer, 4);

osc::Mesh *mesh = osc::MeshBuilder::createPlane(1.0);
osc::Object *object = new osc::Object(mesh);
osc::Camera camera;

void setup() {
  camera.setEye(0, 0, 4.0);
  camera.setCenter(0, 0, 0);
}

float phase = 0;
void loop() {
  object->setRotation(M_PI_2, phase, 0);
  object->setTranslation(sin(phase), 0, 0);
  object->setScaling(sin(phase * 4.0) * 0.25 + 0.75);

  engine.render(&object, 1, camera);

  phase += 0.002;
}
```

More examples can be found in [examples/](examples/) directory.

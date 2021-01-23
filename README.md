# Oscilloscope Graphics for Teensy microcontroller

The library is being developed with [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) (as it's the fastest of the bunch with a dual DAC) but other ones with two DACs might work as well.

![Picture of oscilloscope running an example](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/oscilloscope.jpg)

## Setting up

The library consists of two parts:
- An Arduino/Teensy library that receives line data via serial USB and renders it
- A Python library that generates and sends line data in suitable format to the Teensy

### Teensy

[Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) and [Teensy Loader](https://www.pjrc.com/teensy/loader.html) are required for building and uploading the library.

1. Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder
2. Open _oscilloscope-graphics.ino_ with Teensyduino and compile and upload the sketch

### MacOS/Python

[Python 3](https://www.python.org/) and [Pipenv](https://github.com/pypa/pipenv) are required for installing and running the library and example code. 

1. Install Python 3 with `brew install python3`
2. Install Pipenv with `brew install pipenv`
3. Install dependencies with `pipenv install`
4. Start Python program with `pipenv run python example.py`

## Using external DAC for brightness

If your oscilloscope has z-axis input (i.e. input for setting brightness), an external DAC can be used for setting it. The setup has been tested with [SparkFun MCP4725](https://www.sparkfun.com/products/12918), but most likely any DAC with I2C interface can be made to work with slight modifications. 

On Arduino code level just using `osc::MCP4725Graphics` class should be enough to get the setup working:

```c++
#include <OscilloscopeGraphics.h>

osc::InlineRenderer renderer(10, A21, A22);
osc::MCP4725Graphics gfx(renderer);

void setup() { gfx.start(); }

void loop() { gfx.getAndRenderLines(); }
```

The SparkFun DAC should work by setting up power, ground and output as usually and connecting SDA and SCL to Teensy SDA0 and SCL0 (pins 18 and 19):

![Picture of breadboard with MCP4725](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/breadboard.jpg)

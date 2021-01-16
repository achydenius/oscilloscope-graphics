# Oscilloscope Graphics for Teensy microcontroller

The library is being developed with with [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) but other Teensys with two DACs might work as well.

![Picture of oscilloscope running an example](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/media-lab-demo-day.jpg)

## Setting up

The library consists of two parts:
- An Arduino/Teensy library that receives line data via serial USB and renders it
- A Python library that generates and sends suitable data to the Teensy

### Teensy

[Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) and [Teensy Loader](https://www.pjrc.com/teensy/loader.html) are required for building and uploading the library.

1. Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder
2. Open _oscilloscope-graphics.ino_ with Teensyduino and compile and upload the sketch

## MacOS/Python

[Python 3](https://www.python.org/) and [Pipenv](https://github.com/pypa/pipenv) are required for installing and running the library and example code. 

1. Install Python 3 with `brew install python3`
2. Install Pipenv with `brew install pipenv`
3. Install dependencies with `pipenv install`
4. Start Python program with `pipenv run python example.py`

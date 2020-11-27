# Oscilloscope Graphics for Arduino

Vector graphics renderer for Arduino. The rendered line data is sent to Arduino via USB serial connection which then outputs voltage to oscilloscope via DACs. Currently Python programming language is being used for generating and sending the line data from computer, but other alternatives are also in consideration.

Tested with [Adafruit Metro M4](https://www.adafruit.com/product/3382) but any Arduino compatible board with two DACs will most likely work.

![Picture of oscilloscope running an example](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/media-lab-demo-day.jpg)

## Arduino

Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder

## MacOS example installation

[Python 3](https://www.python.org/) and [Pipenv](https://github.com/pypa/pipenv) are required for installing and running the example.

1. Install Python 3 with `brew install python3`
2. Install Pipenv with `brew install pipenv`
3. Install dependencies with `pipenv install`

## Running

1. Upload `oscilloscope-graphics.ino` to Arduino with Arduino IDE
2. Start Python program with `pipenv run python example.py`

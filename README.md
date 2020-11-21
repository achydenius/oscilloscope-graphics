# Oscilloscope Graphics for Arduino

Vector graphics renderer for Arduino. The rendered line data is sent to Arduino via USB serial connection which then outputs voltage to oscilloscope via DACs. Currently Python programming language is being used for generating and sending the line data from computer, but other alternatives are also in consideration.

Tested with [Adafruit Metro M4](https://www.adafruit.com/product/3382) but any Arduino compatible board with two DACs will most likely work.

![Picture of oscilloscope running an example](https://raw.githubusercontent.com/achydenius/oscilloscope-graphics/master/media-lab-demo-day.jpg)

## Arduino/MacOS installation

1. Install the Oscilloscope Graphics library by moving the _OscilloscopeGraphics_ folder to Arduino _libraries_ folder
2. Install [Python 3](https://www.python.org/) with `brew install python3`
3. Install [pySerial](https://github.com/pyserial/pyserial) with `pip3 install pyserial`

## Running

1. Upload `oscilloscope-graphics.ino` to Arduino with Arduino IDE
2. Start Python program on connected device with `python3 client.py`

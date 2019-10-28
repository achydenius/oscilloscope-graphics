#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include <math.h>
#include <Arduino.h>

class OscilloscopeGraphics
{
    uint8_t resolution;
    uint32_t maxValue;

public:
    OscilloscopeGraphics(uint8_t resolution) : resolution(resolution)
    {
        maxValue = pow(2, resolution) - 1;
        analogWriteResolution(resolution);
    }
    void plot(float x, float y);
};

#endif

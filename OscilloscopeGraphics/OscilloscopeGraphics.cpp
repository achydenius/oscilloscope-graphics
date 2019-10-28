#include "OscilloscopeGraphics.h"

void OscilloscopeGraphics::plot(float x, float y)
{
    analogWrite(A0, (uint16_t)(x * maxValue));
    analogWrite(A1, (uint16_t)(y * maxValue));
}

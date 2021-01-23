#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include <Wire.h>

#include "LineConsumer.h"
#include "Renderer.h"

namespace osc {

class OscilloscopeGraphics {
 protected:
  Renderer& renderer;
  LineConsumer consumer;

  // On faster external DAC blanking point is most likely not needed anymore
  // thus it can be made DefaultGraphics specific
  Point blankingPoint = {0, 0};

 public:
  OscilloscopeGraphics(Renderer& renderer) : renderer(renderer) {}
  OscilloscopeGraphics(Renderer& renderer, Point blankingPoint)
      : renderer(renderer), blankingPoint(blankingPoint) {}

  void start() { consumer.start(); }
  Renderer& getRenderer() { return renderer; }
  LineConsumer& getConsumer() { return consumer; }

  virtual void getAndRenderLines() = 0;
};

class DefaultGraphics : public OscilloscopeGraphics {
 public:
  DefaultGraphics(Renderer& renderer) : OscilloscopeGraphics(renderer) {}
  DefaultGraphics(Renderer& renderer, Point blankingPoint)
      : OscilloscopeGraphics(renderer, blankingPoint) {}

  void getAndRenderLines() {
    Buffer<Line>& lines = consumer.readAndParse();

    for (int i = 0; i < lines.count(); i++) {
      renderer.drawLine(lines[i]);
    }

    renderer.drawPoint(blankingPoint);
  }
};

class MCP4725Graphics : public OscilloscopeGraphics {
  const uint32_t I2C_HIGH_SPEED_MODE_HZ = 3400000;
  uint8_t dacAddress = 0x60;

 public:
  MCP4725Graphics(Renderer& renderer) : OscilloscopeGraphics(renderer) {}
  MCP4725Graphics(Renderer& renderer, uint8_t dacAddress, Point blankingPoint)
      : OscilloscopeGraphics(renderer, blankingPoint), dacAddress(dacAddress) {}

  void start() {
    OscilloscopeGraphics::start();
    Wire.begin();
    Wire.setClock(I2C_HIGH_SPEED_MODE_HZ);
  }

  void getAndRenderLines() {
    Buffer<Line>& lines = consumer.readAndParse();

    unsigned short z = 0;
    for (int i = 0; i < lines.count(); i++) {
      // Set brightness if value has changed
      if (lines[i].z != z) {
        z = lines[i].z;

        // Updating DAC takes a while so move beam to blanking point for
        // avoiding brighter vertices
        renderer.drawPoint(blankingPoint);

        Wire.beginTransmission(dacAddress);
        Wire.write(64);             // Update DAC
        Wire.write(z >> 4);         // The 8 most significant bits
        Wire.write((z & 15) << 4);  // The 4 least significant bits
        Wire.endTransmission();
      }

      renderer.drawLine(lines[i]);
    }

    renderer.drawPoint(blankingPoint);
  }
};

}  // namespace osc

#endif

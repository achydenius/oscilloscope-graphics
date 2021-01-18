#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include <Wire.h>

#include "LineConsumer.h"
#include "Renderer.h"

#define MCP4725_ADDR 0x60

// TODO: Split separate classes for setup with and without external DAC?
namespace osc {
class OscilloscopeGraphics {
  Renderer& renderer;
  LineConsumer& consumer;
  Point blankingPoint = {0, 0};

 public:
  OscilloscopeGraphics(Renderer& renderer, LineConsumer& consumer)
      : renderer(renderer), consumer(consumer) {}

  OscilloscopeGraphics(Renderer& renderer, LineConsumer& consumer,
                       Point blankingPoint)
      : renderer(renderer), consumer(consumer), blankingPoint(blankingPoint) {}

  Renderer& getRenderer() { return renderer; }
  LineConsumer& getConsumer() { return consumer; }

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

        Wire.beginTransmission(MCP4725_ADDR);
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

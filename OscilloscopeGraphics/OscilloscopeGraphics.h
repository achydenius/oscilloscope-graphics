#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include "Renderer.h"
#include "UInt16Consumer.h"

namespace osc {
class OscilloscopeGraphics {
  Renderer& renderer;
  UInt16Consumer& consumer;
  Point<uint16_t> blankingPoint = {0, 0};

 public:
  OscilloscopeGraphics(Renderer& renderer, UInt16Consumer& consumer)
      : renderer(renderer), consumer(consumer) {}

  OscilloscopeGraphics(Renderer& renderer, UInt16Consumer& consumer,
                       Point<uint16_t> blankingPoint)
      : renderer(renderer), consumer(consumer), blankingPoint(blankingPoint) {}

  Renderer& getRenderer() { return renderer; }
  UInt16Consumer& getConsumer() { return consumer; }

  void getAndRenderLines() {
    Buffer<Line<uint16_t>>* lines = consumer.getLines();

    for (int i = 0; i < lines->count(); i++) {
      renderer.drawLine((*lines)[i]);
    }

    renderer.drawPoint(blankingPoint);
  }
};
}  // namespace osc

#endif

#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include "Consumer.h"
#include "Renderer.h"

namespace osc {
class OscilloscopeGraphics {
  Renderer& renderer;
  Consumer& consumer;
  Buffer<Line>* lines;
  Point blankingPoint = {1.0, 1.0};

 public:
  OscilloscopeGraphics(Renderer& renderer, Consumer& consumer)
      : renderer(renderer), consumer(consumer) {}

  OscilloscopeGraphics(Renderer& renderer, Consumer& consumer,
                       Point blankingPoint)
      : renderer(renderer), consumer(consumer), blankingPoint(blankingPoint) {}

  Renderer& getRenderer() { return renderer; }
  Consumer& getConsumer() { return consumer; }

  void getAndRenderLines() {
    lines = consumer.getLines();

    for (int i = 0; i < lines->count(); i++) {
      renderer.drawLine((*lines)[i]);
    }

    renderer.drawPoint(blankingPoint);
  }
};
}  // namespace osc

#endif

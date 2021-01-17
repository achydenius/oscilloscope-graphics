#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include "LineConsumer.h"
#include "Renderer.h"

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
    Buffer<Line>* lines = consumer.readAndParse();

    for (int i = 0; i < lines->count(); i++) {
      renderer.drawLine((*lines)[i]);
    }

    renderer.drawPoint(blankingPoint);
  }
};
}  // namespace osc

#endif

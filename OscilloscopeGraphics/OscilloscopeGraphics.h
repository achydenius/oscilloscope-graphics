#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include "Clipper.h"
#include "Consumer.h"
#include "Renderer.h"

namespace osc {
class OscilloscopeGraphics {
  Renderer& renderer;
  Consumer& consumer;
  Clipper& clipper;
  Buffer<Line>* lines;
  Point blankingPoint = {1.0, 1.0};

 public:
  OscilloscopeGraphics(Renderer& renderer, Consumer& consumer, Clipper& clipper)
      : renderer(renderer), consumer(consumer), clipper(clipper) {}

  OscilloscopeGraphics(Renderer& renderer, Consumer& consumer, Clipper& clipper,
                       Point blankingPoint)
      : renderer(renderer),
        consumer(consumer),
        clipper(clipper),
        blankingPoint(blankingPoint) {}

  Renderer& getRenderer() { return renderer; }
  Consumer& getConsumer() { return consumer; }
  Clipper& getClipper() { return clipper; }

  void getAndRenderLines() {
    lines = consumer.getLines();

    for (int i = 0; i < lines->count(); i++) {
      if (clipper.clipLine((*lines)[i])) {
        renderer.drawLine((*lines)[i]);
      }
    }

    renderer.drawPoint(blankingPoint);
  }
};
}  // namespace osc

#endif

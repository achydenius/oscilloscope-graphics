#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include "Point3DConsumer.h"
#include "Renderer.h"

namespace osc {
class OscilloscopeGraphics {
  Renderer& renderer;
  Point3DConsumer& consumer;
  Buffer<Line<Point3D>>* lines;
  Point2D blankingPoint = {0, 0};

 public:
  OscilloscopeGraphics(Renderer& renderer, Point3DConsumer& consumer)
      : renderer(renderer), consumer(consumer) {}

  OscilloscopeGraphics(Renderer& renderer, Point3DConsumer& consumer,
                       Point2D blankingPoint)
      : renderer(renderer), consumer(consumer), blankingPoint(blankingPoint) {}

  Renderer& getRenderer() { return renderer; }
  Point3DConsumer& getConsumer() { return consumer; }

  void getAndRenderLines() {
    lines = consumer.getLines();

    for (int i = 0; i < lines->count(); i++) {
      renderer.drawDashedLine((*lines)[i]);
    }

    renderer.drawPoint(blankingPoint);
  }
};
}  // namespace osc

#endif

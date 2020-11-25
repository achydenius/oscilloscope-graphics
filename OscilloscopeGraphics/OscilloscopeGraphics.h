#ifndef __OSCILLOSCOPE_GRAPHICS__
#define __OSCILLOSCOPE_GRAPHICS__

#include "Clipper.h"
#include "Renderer.h"
#include "Server.h"

namespace osc {
class OscilloscopeGraphics {
  Renderer& renderer;
  Server& server;
  Clipper& clipper;
  Buffer<Line>* lines;
  Point blankingPoint = {1.0, 1.0};

 public:
  OscilloscopeGraphics(Renderer& renderer, Server& server, Clipper& clipper)
      : renderer(renderer), server(server), clipper(clipper) {}

  OscilloscopeGraphics(Renderer& renderer, Server& server, Clipper& clipper,
                       Point blankingPoint)
      : renderer(renderer),
        server(server),
        clipper(clipper),
        blankingPoint(blankingPoint) {}

  Renderer& getRenderer() { return renderer; }
  Server& getServer() { return server; }
  Clipper& getClipper() { return clipper; }

  void getAndRenderLines() {
    lines = server.getLines();

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

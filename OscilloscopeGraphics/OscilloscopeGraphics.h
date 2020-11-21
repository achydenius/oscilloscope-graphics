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

 public:
  OscilloscopeGraphics(Renderer& renderer, Server& server, Clipper& clipper)
      : renderer(renderer), server(server), clipper(clipper) {}

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
  }
};
}  // namespace osc

#endif

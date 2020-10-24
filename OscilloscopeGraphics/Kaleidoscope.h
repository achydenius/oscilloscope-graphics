#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"
#include "Generator.h"

namespace osc {
class Kaleidoscope {
 public:
  enum Mode { ONE, TWO, FOUR, EIGHT, SIXTEEN };

 private:
  vec2 screenClipVertices[4] = {
      {-1.0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {-1.0, -0.75}};

  Engine engine;
  Mode mode;
  Buffer<Line> clippedLines;
  Clipper clipper;
  ClipPolygon screenPolygon;
  Generator* generator;

 public:
  Kaleidoscope(Renderer& renderer, Mode mode)
      : engine(renderer),
        mode(mode),
        clippedLines(10000),
        screenPolygon(screenClipVertices, 4) {
    switch (mode) {
      case Mode::ONE:
        generator = new OneGenerator(engine);
        break;
      case Mode::TWO:
        generator = new TwoGenerator(engine);
        break;
      case Mode::FOUR:
        generator = new FourGenerator(engine);
        break;
      case Mode::EIGHT:
        generator = new EightGenerator(engine);
        break;
      case Mode::SIXTEEN:
        generator = new SixteenGenerator(engine);
        break;
    }
  };

  void render(Array<Object*>& objects, Camera& camera) {
    Buffer<Line>& generated = generator->generateLines(objects, camera);

    // Modes eight & sixteen require additional clipping against viewport
    Buffer<Line>& lines = (mode == Mode::EIGHT || mode == Mode::SIXTEEN)
                              ? clipLines(generated, screenPolygon)
                              : generated;

    engine.renderLines(lines);

    vec2 blankingPoint = {1.0, 1.0};
    engine.getRenderer().drawPoint(blankingPoint);
  }

 private:
  Buffer<Line>& clipLines(Buffer<Line>& lines, ClipPolygon& polygon) {
    clippedLines.reset();
    for (int i = 0; i < lines.count(); i++) {
      vec2 a, b;
      glm_vec2_copy(lines[i].a, a);
      glm_vec2_copy(lines[i].b, b);

      if (clipper.clipLine(a, b, polygon)) {
        Line line;
        glm_vec2_copy(a, line.a);
        glm_vec2_copy(b, line.b);
        clippedLines.add(line);
      }
    }
    return clippedLines;
  }
};
}  // namespace osc

#endif

#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"

namespace osc {
class Generator {
 protected:
  mat2 mirror22AndHalfDegrees = {{cos(GLM_PI_4f), sin(GLM_PI_4f)},
                                 {sin(GLM_PI_4f), -cos(GLM_PI_4f)}};
  mat2 mirror45Degrees = {{cos(GLM_PI_2f), sin(GLM_PI_2f)},
                          {sin(GLM_PI_2f), -cos(GLM_PI_2f)}};
  mat2 mirrorHorizontal = {{-1.0, 0}, {0, 1.0}};
  mat2 mirrorVertical = {{1.0, 0}, {0, -1.0}};

  Engine& engine;

 public:
  Generator(Engine& engine) : engine(engine){};

  Buffer<Line>& generateLines(Array<Object*>& objects, Camera& camera);

  virtual ClipPolygon& getClipPolygon() = 0;
  virtual Buffer<Line>& mirror(Buffer<Line>& lines) = 0;

 protected:
  void applyMatrix(mat2 matrix, Buffer<Line>& lines);
};

class OneGenerator : public Generator {
  ClipPolygon clipPolygon;
  vec2 clipVertices[4] = {
      {-1.0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {-1.0, -0.75}};

 public:
  OneGenerator(Engine& engine)
      : Generator(engine), clipPolygon(clipVertices, 4){};
  ClipPolygon& getClipPolygon();
  Buffer<Line>& mirror(Buffer<Line>& lines);
};

class TwoGenerator : public Generator {
  ClipPolygon clipPolygon;
  vec2 clipVertices[4] = {{0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {0, -0.75}};

 public:
  TwoGenerator(Engine& engine)
      : Generator(engine), clipPolygon(clipVertices, 4){};
  ClipPolygon& getClipPolygon();
  Buffer<Line>& mirror(Buffer<Line>& lines);
};

class FourGenerator : public Generator {
  ClipPolygon clipPolygon;
  vec2 clipVertices[4] = {{0, 0}, {0, 0.75}, {1.0, 0.75}, {1.0, 0}};

 public:
  FourGenerator(Engine& engine)
      : Generator(engine), clipPolygon(clipVertices, 4){};
  ClipPolygon& getClipPolygon();
  Buffer<Line>& mirror(Buffer<Line>& lines);
};

class EightGenerator : public Generator {
  ClipPolygon clipPolygon;
  vec2 clipVertices[3] = {{0, 0}, {1.0, 1.0}, {1.0, 0}};

 public:
  EightGenerator(Engine& engine)
      : Generator(engine), clipPolygon(clipVertices, 3){};
  ClipPolygon& getClipPolygon();
  Buffer<Line>& mirror(Buffer<Line>& lines);
};

class SixteenGenerator : public Generator {
  ClipPolygon clipPolygon;
  vec2 clipVertices[3] = {{0, 0}, {1.0, 0.5}, {1.0, 0}};

 public:
  SixteenGenerator(Engine& engine)
      : Generator(engine), clipPolygon(clipVertices, 3) {}
  ClipPolygon& getClipPolygon();
  Buffer<Line>& mirror(Buffer<Line>& lines);
};

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

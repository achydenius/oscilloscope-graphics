#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"

namespace osc {
class Kaleidoscope {
 public:
  enum Mode { ONE, TWO, FOUR, EIGHT, SIXTEEN };

 private:
  vec2 modeOneClipVertices[4] = {
      {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}, {-1.0, -1.0}};
  vec2 modeTwoClipVertices[4] = {{0, 1.0}, {1.0, 1.0}, {1.0, -1.0}, {0, -1.0}};
  vec2 modeFourClipVertices[4] = {{0, 0}, {0, 1.0}, {1.0, 1.0}, {1.0, 0}};
  vec2 modeEightClipVertices[3] = {{0, 0}, {1.0, 1.0}, {1.0, 0}};
  vec2 modeSixteenClipVertices[3] = {{0, 0}, {1.0, 0.5}, {1.0, 0}};

  mat2 mirror22AndHalfDegrees = {{cos(GLM_PI_4f), sin(GLM_PI_4f)},
                                 {sin(GLM_PI_4f), -cos(GLM_PI_4f)}};
  mat2 mirror45Degrees = {{cos(GLM_PI_2f), sin(GLM_PI_2f)},
                          {sin(GLM_PI_2f), -cos(GLM_PI_2f)}};
  mat2 mirrorHorizontal = {{-1.0, 0}, {0, 1.0}};
  mat2 mirrorVertical = {{1.0, 0}, {0, -1.0}};

  Engine engine;
  Mode mode;
  ClipPolygon* poly;
  Buffer<Line> lines;

 public:
  Kaleidoscope(Renderer& renderer, Mode mode)
      : engine(renderer), mode(mode), lines(10000) {
    switch (mode) {
      case Mode::ONE:
        poly = new ClipPolygon(modeOneClipVertices, 4);
        break;
      case Mode::TWO:
        poly = new ClipPolygon(modeTwoClipVertices, 4);
        break;
      case Mode::FOUR:
        poly = new ClipPolygon(modeFourClipVertices, 4);
        break;
      case Mode::EIGHT:
        poly = new ClipPolygon(modeEightClipVertices, 3);
        break;
      case Mode::SIXTEEN:
        poly = new ClipPolygon(modeSixteenClipVertices, 3);
        break;
    }
    engine.setViewport(poly);
  };

  void render(Array<Object*>& objects, Camera& camera) {
    lines.reset();
    Buffer<Object*>& transformed = engine.transformObjects(objects, camera);
    Buffer<Line>& lines = engine.clipObjects(transformed);

    switch (mode) {
      case Mode::TWO:
        mirrorTwo(lines);
        break;
      case Mode::FOUR:
        mirrorFour(lines);
        break;
      case Mode::EIGHT:
        mirrorEight(lines);
        break;
      case Mode::SIXTEEN:
        mirrorSixteen(lines);
        break;
    }
    engine.renderLines(lines);

    vec2 blankingPoint = {1.0, 1.0};
    engine.getRenderer().drawPoint(blankingPoint);
  }

 private:
  void mirrorTwo(Buffer<Line>& lines) { mirror(mirrorHorizontal, lines); }

  void mirrorFour(Buffer<Line>& lines) {
    mirror(mirrorHorizontal, lines);
    mirror(mirrorVertical, lines);
  }

  void mirrorEight(Buffer<Line>& lines) {
    mirror(mirror45Degrees, lines);
    mirror(mirrorHorizontal, lines);
    mirror(mirrorVertical, lines);
  }

  void mirrorSixteen(Buffer<Line>& lines) {
    mirror(mirror22AndHalfDegrees, lines);
    mirror(mirror45Degrees, lines);
    mirror(mirrorHorizontal, lines);
    mirror(mirrorVertical, lines);
  }

  void mirror(mat2 matrix, Buffer<Line>& lines) {
    for (int i = lines.count() - 1; i >= 0; i--) {
      Line line;
      glm_mat2_mulv(matrix, lines[i].a, line.a);
      glm_mat2_mulv(matrix, lines[i].b, line.b);
      lines.add(line);
    }
  }
};
}  // namespace osc

#endif

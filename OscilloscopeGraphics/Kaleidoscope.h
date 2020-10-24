#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"

namespace osc {
class Kaleidoscope {
 public:
  enum Mode { ONE, TWO, FOUR, EIGHT, SIXTEEN };

 private:
  vec2 screenClipVertices[4] = {
      {-1.0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {-1.0, -0.75}};
  vec2 modeTwoClipVertices[4] = {
      {0, 0.75}, {1.0, 0.75}, {1.0, -0.75}, {0, -0.75}};
  vec2 modeFourClipVertices[4] = {{0, 0}, {0, 0.75}, {1.0, 0.75}, {1.0, 0}};
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
  ClipPolygon* clipPolygon;
  Buffer<Line> clippedLines;
  Clipper clipper;
  ClipPolygon screenPolygon;

 public:
  Kaleidoscope(Renderer& renderer, Mode mode)
      : engine(renderer),
        mode(mode),
        clippedLines(10000),
        screenPolygon(screenClipVertices, 4) {
    switch (mode) {
      case Mode::ONE:
        clipPolygon = new ClipPolygon(screenClipVertices, 4);
        break;
      case Mode::TWO:
        clipPolygon = new ClipPolygon(modeTwoClipVertices, 4);
        break;
      case Mode::FOUR:
        clipPolygon = new ClipPolygon(modeFourClipVertices, 4);
        break;
      case Mode::EIGHT:
        clipPolygon = new ClipPolygon(modeEightClipVertices, 3);
        break;
      case Mode::SIXTEEN:
        clipPolygon = new ClipPolygon(modeSixteenClipVertices, 3);
        break;
    }
    engine.setViewport(clipPolygon);
  };

  void render(Array<Object*>& objects, Camera& camera) {
    Buffer<Line>& generated = generateLines(objects, camera);

    // Modes eight & sixteen require additional clipping against viewport
    Buffer<Line>& lines = (mode == Mode::EIGHT || mode == Mode::SIXTEEN)
                              ? clipLines(generated, screenPolygon)
                              : generated;

    engine.renderLines(lines);

    vec2 blankingPoint = {1.0, 1.0};
    engine.getRenderer().drawPoint(blankingPoint);
  }

 private:
  Buffer<Line>& generateLines(Array<Object*>& objects, Camera& camera) {
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

    return lines;
  }

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

#ifndef __KALEIDOSCOPE__
#define __KALEIDOSCOPE__

#include "Engine.h"

namespace osc {
class Kaleidoscope {
 public:
  enum Mode { TWO, FOUR, EIGHT };

 private:
  Engine engine;
  Mode mode;
  vec2 vertices[3] = {{0, 0}, {1.0, 1.0}, {1.0, 0}};
  ClipPolygon* poly;
  Buffer<Line> lines;

 public:
  Kaleidoscope(Renderer& renderer, Mode mode)
      : engine(renderer), mode(mode), lines(10000) {
    poly = new ClipPolygon(vertices, 3);
    engine.setViewport(poly);
  };

  void render(Array<Object*>& objects, Camera& camera) {
    lines.reset();

    Buffer<Object*>& transformed = engine.transformObjects(objects, camera);
    Buffer<Line>& lines = engine.clipObjects(transformed);

    mat2 mirror45degrees = {{cos(GLM_PI_2f), sin(GLM_PI_2f)},
                            {sin(GLM_PI_2f), -cos(GLM_PI_2f)}};
    mat2 mirrorHorizontal = {{-1.0, 0}, {0, 1.0}};
    mat2 mirrorVertical = {{1.0, 0}, {0, -1.0}};

    mirror(mirror45degrees, lines);
    mirror(mirrorHorizontal, lines);
    mirror(mirrorVertical, lines);

    engine.renderLines(lines);

    vec2 blankingPoint = {1.0, 1.0};
    engine.getRenderer().drawPoint(blankingPoint);
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

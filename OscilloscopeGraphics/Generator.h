#ifndef __GENERATOR__
#define __GENERATOR__

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
}  // namespace osc

#endif

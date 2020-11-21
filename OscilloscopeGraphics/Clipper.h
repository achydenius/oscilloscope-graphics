#ifndef __CLIPPER__
#define __CLIPPER__

#include "Renderer.h"

namespace osc {
class ClipPolygon {
 public:
  int vertexCount;
  Point* vertices;
  Point* normals;

  ClipPolygon(Point* vertices, int vertexCount)
      : vertices(vertices), vertexCount(vertexCount) {
    normals = new Point[vertexCount];
    calculateNormals();
  };
  ~ClipPolygon();

 private:
  void calculateNormals();
};

class Clipper {
  static const int defaultVertexCount = 4;
  Point defaultVertices[defaultVertexCount] = {
      {-1.0, 0.5}, {1.0, 0.5}, {1.0, -0.5}, {-1.0, -0.5}};
  ClipPolygon* viewport;

 public:
  Clipper() { viewport = new ClipPolygon(defaultVertices, defaultVertexCount); }
  Clipper(ClipPolygon& polygon) { viewport = &polygon; }

  bool clipLine(Line& line);

 private:
  float minValue(float a, float b);
  float maxValue(float a, float b);
};
}  // namespace osc

#endif

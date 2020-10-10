#ifndef __CLIPPER__
#define __CLIPPER__

#include "Renderer.h"

namespace osc {
class ClipPolygon {
 public:
  int vertexCount;
  vec2* vertices;
  vec2* normals;

  ClipPolygon(vec2* vertices, int vertexCount)
      : vertices(vertices), vertexCount(vertexCount) {
    normals = new vec2[vertexCount];
    calculateNormals();
  };
  ~ClipPolygon();

 private:
  void calculateNormals();
};

class Clipper {
 public:
  bool clipLine(vec2& a, vec2& b, ClipPolygon& polyggon);
};
}  // namespace osc

#endif

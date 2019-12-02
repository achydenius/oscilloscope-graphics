#ifndef __MESH__
#define __MESH__

#include "src/cglm/include/cglm/cglm.h"

namespace osc {
struct Edge {
  int a, b;
};

class Mesh {
 public:
  int vertexCount;
  int edgeCount;
  vec3* vertices;
  Edge* edges;
  vec4 boundingSphere;

  Mesh(vec3* vertices, int vertexCount, Edge* edges, int edgeCount);
  ~Mesh();

 private:
  void calculateBoundingSphere();
};

namespace MeshBuilder {
Mesh* createCube(float size);
Mesh* createPlane(float size);
}  // namespace MeshBuilder
}  // namespace osc

#endif

#include "Mesh.h"

namespace osc {
Mesh::Mesh(vec3* sourceVertices, int sourceVertexCount, Edge* sourceEdges,
           int sourceEdgeCount) {
  vertexCount = sourceVertexCount;
  edgeCount = sourceEdgeCount;
  vertices = new vec3[vertexCount];
  edges = new Edge[edgeCount];

  for (int i = 0; i < vertexCount; i++) {
    glm_vec3_copy(sourceVertices[i], vertices[i]);
  }
  for (int i = 0; i < edgeCount; i++) {
    edges[i] = sourceEdges[i];
  }

  calculateBoundingSphere();
}

Mesh::~Mesh() {
  delete vertices;
  delete edges;
}

void Mesh::calculateBoundingSphere() {
  vec3 min = {0, 0, 0};
  vec3 max = {0, 0, 0};

  // Calculate axis-aligned bounding box
  for (int i = 0; i < vertexCount; i++) {
    min[0] = glm_min(min[0], vertices[i][0]);
    max[0] = glm_max(max[0], vertices[i][0]);
    min[1] = glm_min(min[1], vertices[i][1]);
    max[1] = glm_max(max[1], vertices[i][1]);
    min[2] = glm_min(min[2], vertices[i][2]);
    max[2] = glm_max(max[2], vertices[i][2]);
  }

  // Calculate center and radius of the sphere
  vec3 center;
  float r = 0;
  glm_vec3_center(min, max, center);
  for (int i = 0; i < vertexCount; i++) {
    r = glm_max(r, glm_vec3_distance(vertices[i], center));
  }

  glm_vec4(center, r, boundingSphere);
}

namespace MeshBuilder {
Mesh* createCube(float size) {
  float half = size / 2;
  vec3 vertices[] = {{-half, half, half},  {-half, half, -half},
                     {half, half, -half},  {half, half, half},
                     {-half, -half, half}, {-half, -half, -half},
                     {half, -half, -half}, {half, -half, half}};
  Edge edges[] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6},
                  {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

  return new Mesh(vertices, 8, edges, 12);
}

Mesh* createPlane(float size) {
  float half = size / 2;
  vec3 vertices[] = {
      {-half, 0, half}, {-half, 0, -half}, {half, 0, -half}, {half, 0, half}};
  Edge edges[] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};

  return new Mesh(vertices, 4, edges, 4);
}
}  // namespace MeshBuilder
}  // namespace osc

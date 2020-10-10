#include "Clipper.h"

using namespace osc;

ClipPolygon::~ClipPolygon() { delete normals; }

void ClipPolygon::calculateNormals() {
  for (int i = 0; i < vertexCount; i++) {
    vec2 dir;
    glm_vec2_sub(vertices[(i + 1) % vertexCount], vertices[i], dir);
    normals[i][0] = -dir[1];
    normals[i][1] = dir[0];
    glm_vec2_normalize(normals[i]);
  }
};

/*
 * Clip line against a convex polygon
 *
 * Cyrus-Beck line clipping algorithm implementation:
 * https://en.wikipedia.org/wiki/Cyrus%E2%80%93Beck_algorithm
 */
bool Clipper::clipLine(vec2& a, vec2& b, ClipPolygon& polygon) {
  float tE = 0, tL = 1;
  vec2 P1_P0, PEi;
  glm_vec2_sub(b, a, P1_P0);

  for (int i = 0; i < polygon.vertexCount; i++) {
    glm_vec2_sub(a, polygon.vertices[i], PEi);
    float num = glm_vec2_dot(polygon.normals[i], PEi);
    float den = glm_vec2_dot(polygon.normals[i], P1_P0);

    if (den != 0) {
      float t = num / -den;

      if (den < 0) {
        tE = glm_max(tE, t);  // Entering
      } else {
        tL = glm_min(tL, t);  // Exiting
      }
    } else if (num > 0) {  // Parallel and outside
      return false;
    }
  }

  if (tE > tL) {  // Outside
    return false;
  }

  float x0 = a[0];
  float y0 = a[1];

  a[0] = x0 + P1_P0[0] * tE;
  a[1] = y0 + P1_P0[1] * tE;
  b[0] = x0 + P1_P0[0] * tL;
  b[1] = y0 + P1_P0[1] * tL;
  return true;
}

#include "Clipper.h"

using namespace osc;

ClipPolygon::~ClipPolygon() { delete normals; }

void ClipPolygon::calculateNormals() {
  for (int i = 0; i < vertexCount; i++) {
    Point dir;
    (vertices[(i + 1) % vertexCount]).sub(vertices[i], dir);
    normals[i].x = -dir.y;
    normals[i].y = dir.x;
    normals[i].normalize();
  }
};

/*
 * Clip line against a convex polygon
 *
 * Cyrus-Beck line clipping algorithm implementation:
 * https://en.wikipedia.org/wiki/Cyrus%E2%80%93Beck_algorithm
 */
bool Clipper::clipLine(Line& line) {
  float tE = 0, tL = 1;
  Point P1_P0, PEi;
  line.b.sub(line.a, P1_P0);

  for (int i = 0; i < viewport->vertexCount; i++) {
    line.a.sub(viewport->vertices[i], PEi);
    float num = viewport->normals[i].dot(PEi);
    float den = viewport->normals[i].dot(P1_P0);

    if (den != 0) {
      float t = num / -den;

      if (den < 0) {
        tE = maxValue(tE, t);  // Entering
      } else {
        tL = minValue(tL, t);  // Exiting
      }
    } else if (num > 0) {  // Parallel and outside
      return false;
    }
  }

  if (tE > tL) {  // Outside
    return false;
  }

  float x0 = line.a.x;
  float y0 = line.a.y;

  line.a.x = x0 + P1_P0.x * tE;
  line.a.y = y0 + P1_P0.y * tE;
  line.b.x = x0 + P1_P0.x * tL;
  line.b.y = y0 + P1_P0.y * tL;
  return true;
}

float Clipper::minValue(float a, float b) { return a < b ? a : b; }

float Clipper::maxValue(float a, float b) { return a > b ? a : b; }

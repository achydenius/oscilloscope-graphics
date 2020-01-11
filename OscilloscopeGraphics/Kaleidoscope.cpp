#include "Kaleidoscope.h"

using namespace osc;

void Kaleidoscope::render(Object** objects, int objectCount, Camera& camera) {
  if (mode == Mode::HORIZONTAL) {
    Renderer::Viewport viewports[] = {{1, -1, 0, 1}, {1, -1, -1, 0}};
    mat4 matrices[] = {
        GLM_MAT4_IDENTITY_INIT,
        {{-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    for (int i = 0; i < 2; i++) {
      renderer->setViewport(viewports[i]);
      renderObjects(objects, objectCount, camera, &matrices[i]);
    }

  } else if (mode == Mode::VERTICAL) {
    Renderer::Viewport viewports[] = {{1, 0, -1, 1}, {0, -1, -1, 1}};
    mat4 matrices[] = {
        GLM_MAT4_IDENTITY_INIT,
        {{1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    for (int i = 0; i < 2; i++) {
      renderer->setViewport(viewports[i]);
      renderObjects(objects, objectCount, camera, &matrices[i]);
    }

  } else if (mode == Mode::QUAD) {
    Renderer::Viewport viewports[] = {
        {1, 0, 0, 1}, {0, -1, 0, 1}, {0, -1, -1, 0}, {1, 0, -1, 0}};
    mat4 matrices[] = {
        GLM_MAT4_IDENTITY_INIT,
        {{1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
        {{-1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
        {{-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    for (int i = 0; i < 4; i++) {
      renderer->setViewport(viewports[i]);
      renderObjects(objects, objectCount, camera, &matrices[i]);
    }
  }
}

/*
 * Clip line against a convex polygon
 *
 * Cyrus-Beck line clipping algorithm implementation:
 * https://en.wikipedia.org/wiki/Cyrus%E2%80%93Beck_algorithm
 */
void Kaleidoscope::clipLine(float x0, float y0, float x1, float y1) {
  vec3 p1p0 = {x1 - x0, y1 - y0, 0};
  float t = 0, tE = 0, tL = 1;

  for (int i = 0; i < clipVertexCount; i++) {
    vec3 p0pei = {x0 - clipVertices[i][0], y0 - clipVertices[i][1], 0};
    float numerator = glm_vec3_dot(clipNormals[i], p0pei);
    float denominator = glm_vec3_dot(clipNormals[i], p1p0);

    if (denominator != 0) {
      t = numerator / -denominator;

      if (denominator < 0) {
        tE = glm_max(tE, t);  // Entering
      } else {
        tL = glm_min(tL, t);  // Leaving
      }
    } else if (numerator > 0) {  // Parallel and outside
      return;
    }
  }

  if (tE > tL) {  // Outside
    return;
  }

  vec3 p0, p1;
  p0[0] = x0 + p1p0[0] * tE;
  p0[1] = y0 + p1p0[1] * tE;
  p1[0] = x0 + p1p0[0] * tL;
  p1[1] = y0 + p1p0[1] * tL;
}

void Kaleidoscope::calculateClipNormals() {
  for (int i = 0; i < clipVertexCount; i++) {
    vec3 z = {0, 0, -1};
    vec3 dir;
    glm_vec3_sub(clipVertices[(i + 1) % clipVertexCount], clipVertices[i], dir);
    glm_vec3_crossn(dir, z, clipNormals[i]);
  }
}

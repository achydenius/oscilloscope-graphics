#include "Kaleidoscope.h"

using namespace osc;

Buffer<Line>& Generator::generateLines(Array<Object*>& objects,
                                       Camera& camera) {
  engine.setViewport(&getClipPolygon());
  Buffer<Object*>& transformed = engine.transformObjects(objects, camera);
  Buffer<Line>& clipped = engine.clipObjects(transformed);
  mirror(clipped);

  return clipped;
}

void Generator::applyMatrix(mat2 matrix, Buffer<Line>& lines) {
  for (int i = lines.count() - 1; i >= 0; i--) {
    Line line;
    glm_mat2_mulv(matrix, lines[i].a, line.a);
    glm_mat2_mulv(matrix, lines[i].b, line.b);
    lines.add(line);
  }
}

ClipPolygon& OneGenerator::getClipPolygon() { return clipPolygon; }

Buffer<Line>& OneGenerator::mirror(Buffer<Line>& lines) { return lines; }

ClipPolygon& TwoGenerator::getClipPolygon() { return clipPolygon; }

Buffer<Line>& TwoGenerator::mirror(Buffer<Line>& lines) {
  applyMatrix(mirrorHorizontal, lines);
  return lines;
}

ClipPolygon& FourGenerator::getClipPolygon() { return clipPolygon; }

Buffer<Line>& FourGenerator::mirror(Buffer<Line>& lines) {
  applyMatrix(mirrorHorizontal, lines);
  applyMatrix(mirrorVertical, lines);
  return lines;
}

ClipPolygon& EightGenerator::getClipPolygon() { return clipPolygon; }

Buffer<Line>& EightGenerator::mirror(Buffer<Line>& lines) {
  applyMatrix(mirror45Degrees, lines);
  applyMatrix(mirrorHorizontal, lines);
  applyMatrix(mirrorVertical, lines);
  return lines;
}

ClipPolygon& SixteenGenerator::getClipPolygon() { return clipPolygon; }

Buffer<Line>& SixteenGenerator::mirror(Buffer<Line>& lines) {
  applyMatrix(mirror22AndHalfDegrees, lines);
  applyMatrix(mirror45Degrees, lines);
  applyMatrix(mirrorHorizontal, lines);
  applyMatrix(mirrorVertical, lines);
  return lines;
}

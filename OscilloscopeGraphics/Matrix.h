#ifndef __MATRIX__
#define __MATRIX__

#include <math.h>

namespace osc {
class Vector3D {
 public:
  float x, y, z;

  void set(float nx, float ny, float nz) {
    x = nx;
    y = ny;
    z = nz;
  }

  bool equals(Vector3D& vector) {
    return x == vector.x && y == vector.y && z == vector.z;
  }

  void subtract(Vector3D& a, Vector3D& b) {
    x = a.x - b.x;
    y = a.y - b.y;
    z = a.z - b.z;
  }

  float length() { return sqrt(x * x + y * y + z * z); }

  void normalize() {
    float len = length();
    if (len != 0) {
      x /= len;
      y /= len;
      z /= len;
    }
  }

  void cross(Vector3D& a, Vector3D& b) {
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;
  }

  float dot(Vector3D& vector) {
    return x * vector.x + y * vector.y + z * vector.z;
  }
};

class Vector2D {
 public:
  float x, y;

  void project(Vector3D& vector, int d) {
    x = vector.x / (vector.z + d);
    y = vector.y / (vector.z + d);
  }
};

class Matrix {
  float m[4][4];

 public:
  Matrix() { identity(); }

  void identity() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        m[i][j] = i == j ? 1.0 : 0;
      }
    }
  }

  void rotation(float x, float y, float z) {
    float sinX = sin(x);
    float cosX = cos(x);
    float sinY = sin(y);
    float cosY = cos(y);
    float sinZ = sin(z);
    float cosZ = cos(z);

    m[0][0] = cosY * cosZ;
    m[0][1] = -cosY * sinZ;
    m[0][2] = sinY;
    m[0][3] = 0;

    m[1][0] = sinX * sinY * cosZ + cosX * sinZ;
    m[1][1] = -sinX * sinY * sinZ + cosX * cosZ;
    m[1][2] = -sinX * cosY;
    m[1][3] = 0;

    m[2][0] = -cosX * sinY * cosZ + sinX * sinZ;
    m[2][1] = cosX * sinY * sinZ + sinX * cosZ;
    m[2][2] = cosX * cosY;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1.0;
  }

  void translation(float x, float y, float z) {
    identity();
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
  }

  void scaling(float scale) {
    identity();
    m[0][0] = scale;
    m[1][1] = scale;
    m[2][2] = scale;
  }

  void lookAt(Vector3D& eye, Vector3D& target) {
    Vector3D forward, side;
    Vector3D up = {0, 1.0, 0};

    forward.subtract(target, eye);
    forward.normalize();
    side.cross(forward, up);
    side.normalize();
    up.cross(side, forward);
    up.normalize();

    m[0][0] = side.x;
    m[0][1] = up.x;
    m[0][2] = -forward.x;
    m[0][3] = -eye.dot(side);

    m[1][0] = side.y;
    m[1][1] = up.y;
    m[1][2] = -forward.y;
    m[1][3] = -eye.dot(up);

    m[2][0] = side.z;
    m[2][1] = up.z;
    m[2][2] = -forward.z;
    m[2][3] = -eye.dot(forward);

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1.0;
  }

  void multiply(Matrix& a, Matrix& b) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] +
                  a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
      }
    }
  }

  void multiply(Vector3D& source, Vector3D& target) {
    target.x = (m[0][0] * source.x) + (m[0][1] * source.y) +
               (m[0][2] * source.z) + m[0][3];
    target.y = (m[1][0] * source.x) + (m[1][1] * source.y) +
               (m[1][2] * source.z) + m[1][3];
    target.z = (m[2][0] * source.x) + (m[2][1] * source.y) +
               (m[2][2] * source.z) + m[2][3];
  }
};  // namespace osc
}  // namespace osc
#endif

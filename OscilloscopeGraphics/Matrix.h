#ifndef __MATRIX__
#define __MATRIX__

#include <math.h>

struct Vector2D {
  float x, y;
};

class Vector3D {
 public:
  float x, y, z;

  void project(Vector2D& target, int d) {
    target.x = x / (z + d);
    target.y = y / (z + d);
  }
};

class Matrix {
  float m[3][3];

 public:
  Matrix() { identity(); }

  void identity() {
    m[0][0] = 1.0;
    m[0][1] = 0;
    m[0][2] = 0;
    m[1][0] = 0;
    m[1][1] = 1.0;
    m[1][2] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1.0;
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

    m[1][0] = sinX * sinY * cosZ + cosX * sinZ;
    m[1][1] = -sinX * sinY * sinZ + cosX * cosZ;
    m[1][2] = -sinX * cosY;

    m[2][0] = -cosX * sinY * cosZ + sinX * sinZ;
    m[2][1] = cosX * sinY * sinZ + sinX * cosZ;
    m[2][2] = cosX * cosY;
  }

  void scaling(float scale) {
    m[0][0] = scale;
    m[0][1] = 0;
    m[0][2] = 0;
    m[1][0] = 0;
    m[1][1] = scale;
    m[1][2] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = scale;
  }

  void multiply(Matrix& a, Matrix& b) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        float sum = 0;
        for (int k = 0; k < 3; k++) {
          sum += a.m[i][k] * b.m[k][j];
        }
        m[i][j] = sum;
      }
    }
  }

  void transform(Vector3D& source, Vector3D& target) {
    target.x =
        (m[0][0] * source.x) + (m[0][1] * source.y) + (m[0][2] * source.z);
    target.y =
        (m[1][0] * source.x) + (m[1][1] * source.y) + (m[1][2] * source.z);
    target.z =
        (m[2][0] * source.x) + (m[2][1] * source.y) + (m[2][2] * source.z);
  }
};

#endif

#ifndef __TYPES__
#define __TYPES__

#include <math.h>

namespace osc {

class Point {
 public:
  float x, y;

  float dot(Point& point) { return (x * point.x) + (y * point.y); }

  void sub(Point& point, Point& result) {
    result.x = x - point.x;
    result.y = y - point.y;
  };

  float length() { return sqrt((x * x) + (y * y)); }

  void normalize() {
    float len = length();
    x /= len;
    y /= len;
  }
};

struct Line {
  Point a, b;
};

template <typename T>
class Buffer {
  T* data;
  int unsigned size;
  int unsigned index;

 public:
  Buffer(int unsigned size) : size(size) { data = new T[size]; };
  ~Buffer() { delete data; }

  T& operator[](int unsigned index) { return data[index]; }
  int count() { return index; }
  void reset() { index = 0; }
  void add(T d) { data[index++] = d; }
};

}  // namespace osc

#endif

#ifndef __TYPES__
#define __TYPES__

#include <math.h>

namespace osc {

template <typename T>
struct Point {
  T x, y;
};

template <typename T>
struct Line {
  Point<T> a, b;
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

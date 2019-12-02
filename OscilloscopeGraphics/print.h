#ifndef __PRINT__
#define __PRINT__

#include "src/cglm/include/cglm/cglm.h"

void vec3print(vec3 vector) {
  for (int i = 0; i < 3; i++) {
    Serial.print(vector[i]);
    Serial.print("\t");
  }
  Serial.print("\n");
}

void vec4print(vec4 vector) {
  for (int i = 0; i < 4; i++) {
    Serial.print(vector[i]);
    Serial.print("\t");
  }
  Serial.print("\n");
}

void mat4print(mat4 matrix) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      Serial.print(matrix[i][j]);
      Serial.print("\t");
    }
    Serial.print("\n");
  }
}

#endif

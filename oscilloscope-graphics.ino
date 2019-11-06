#include <math.h>
#include <OscilloscopeGraphics.h>

/* Floating point map implementation */
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* Clamp value to max or min value depending on approach direction */
float clamp(float value, float target, float dir) {
  if ((dir > 0 && value > target) || (dir < 0 && value < target)) {
    return target;
  } else {
    return value;
  }
}

/* Class for a randomly moving cube object */
class Cube : public osc::Object {
public:
  osc::Vector3D pos;
  osc::Vector3D target;
  osc::Vector3D dir;
  int waitAfterTarget;
  int wait;
  
  const float velocity;

  Cube(osc::Mesh *mesh, float velocity, float waitAfterTarget)
      : osc::Object(mesh), velocity(velocity), waitAfterTarget(waitAfterTarget) {};

  void randomizePosition() {
    pos.set(random(4), 0, random(4));
  }

  void randomizeTarget() {
    if (random(2)) {
      target.set(random(4), pos.y, pos.z);
      dir.set(pos.x < target.x ? velocity : -velocity, 0, 0);
    } else {
      target.set(pos.x, pos.y, random(4));
      dir.set(0, 0, pos.z < target.z ? velocity : -velocity);
    }
  }

  void update() {
    if (wait > 0) {
      wait--;
      return;
    }
    
    pos.set(
      clamp((pos.x + dir.x), target.x, dir.x),
      clamp((pos.y + dir.y), target.y, dir.y),
      clamp((pos.z + dir.z), target.z, dir.z)
    );

    if (pos.equals(target)) {
      randomizeTarget();
      wait = waitAfterTarget;
    }
  }
};

osc::Engine engine(10, A0, A1, 8);

osc::Vector3D vertices[] = {
  { -10.0, 10.0, 10.0 },
  { -10.0, 10.0, -10.0 },
  { 10.0, 10.0, -10.0 },
  { 10.0, 10.0, 10.0 },
  { -10.0, -10.0, 10.0 },
  { -10.0, -10.0, -10.0 },
  { 10.0, -10.0, -10.0 },
  { 10.0, -10.0, 10.0 }
};
osc::Edge edges[] = {
  { 0, 1 },
  { 1, 2 },
  { 2, 3 },
  { 3, 0 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },
  { 7, 4 },
  { 0, 4 },
  { 1, 5 },
  { 2, 6 },
  { 3, 7 }
};
osc::Mesh mesh = {
  8, 12, vertices, edges
};

const int cubeCount = 2;
Cube cubes[] = {
  Cube(&mesh, 0.01, 100),
  Cube(&mesh, 0.005, 100)
};
osc::Matrix camera;

void setup() {
  randomSeed(millis());

  osc::Vector3D eye = { 0, 20.0, 40.0 };
  osc::Vector3D target = { 0, 0, 0 };
  camera.lookAt(eye, target);

  for (int i = 0; i < cubeCount; i++) {
    cubes[i].randomizePosition();
    cubes[i].randomizeTarget();
  }
}

void loop() {
  for (int i = 0; i < cubeCount; i++) {
    cubes[i].setTranslation(
      mapf(cubes[i].pos.x, 0, 3, -30.0, 30.0),
      0,
      mapf(cubes[i].pos.z, 0, 3, -30.0, 30.0)
    );  
    engine.render(cubes[i], camera, 10.0);
    cubes[i].update();
  }
}

#include <math.h>
#include <OscilloscopeGraphics.h>

/* Return a pseudo-random boolean value */
bool randomBoolean() {
  return random(2);
}

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
  bool moving;
  
  const float velocity;

  Cube(osc::Mesh *mesh, float velocity, float waitAfterTarget)
      : osc::Object(mesh), velocity(velocity), waitAfterTarget(waitAfterTarget) {};

  void randomizeTarget(int sceneSize, Cube& other) {
    // Move along x-axis
    if (randomBoolean()) {
      // If other cube is on the same row, limit movement to free cells
      if (pos.z == other.pos.z) {
        int min = other.pos.x < pos.x ? other.pos.x + 1 : 0;
        int max = other.pos.x > pos.x ? other.pos.x : sceneSize;
        target.set(random(min, max), pos.y, pos.z);         
      // Otherwise move anywhere on the same row
      } else {
        target.set(random(sceneSize), pos.y, pos.z);
      }
      
      dir.set(pos.x < target.x ? velocity : -velocity, 0, 0);

    // Move along z-axis
    } else {
      // If other cube is on the same column, limit movement to free cells
      if (pos.x == other.pos.x) {
        int min = other.pos.z < pos.z ? other.pos.z + 1 : 0;
        int max = other.pos.z > pos.z ? other.pos.z : sceneSize;
        target.set(pos.x, pos.y, random(min, max));
      // Otherwise move anywhere on the same col
      } else {
        target.set(pos.x, pos.y, random(sceneSize));
      }
      dir.set(0, 0, pos.z < target.z ? velocity : -velocity);
    }
  }

  void update(int sceneSize, Cube& other) {
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
      other.randomizeTarget(sceneSize, *this);
      wait = waitAfterTarget;
      moving = false;
      other.moving = true;
    }
  }
};

/* Class for a scene with moving cubes */
class Scene {
  const int size;
  int cubeCount;
  Cube *cubes;
  osc::Object *plane;
  osc::Engine& engine;

public:
  Scene(osc::Engine& engine, Cube *cubes, int cubeCount, osc::Object *plane, int size)
      : engine(engine), cubes(cubes), cubeCount(cubeCount), plane(plane), size(size) {}

  void init() {
    cubes[0].pos.set(0, 0, 0);
    cubes[1].pos.set(size - 1, 0, size - 1);
    cubes[0].randomizeTarget(size, cubes[1]);
    cubes[0].moving = true;
  }

  void process(osc::Matrix& camera) {
    for (int i = 0; i < cubeCount; i++) {
      cubes[i].setTranslation(
        mapf(cubes[i].pos.x, 0, 3, -30.0, 30.0),
        0,
        mapf(cubes[i].pos.z, 0, 3, -30.0, 30.0)
      );  
      engine.render(cubes[i], camera, 20.0);

      if (cubes[i].moving) {
        cubes[i].update(size, cubes[i ? 0 : 1]);
      }
    }

    engine.render(*plane, camera, 20.0);
  }
};

osc::Engine engine(10, A0, A1, 8);

osc::Vector3D cubeVertices[] = {
  { -10.0, 10.0, 10.0 },
  { -10.0, 10.0, -10.0 },
  { 10.0, 10.0, -10.0 },
  { 10.0, 10.0, 10.0 },
  { -10.0, -10.0, 10.0 },
  { -10.0, -10.0, -10.0 },
  { 10.0, -10.0, -10.0 },
  { 10.0, -10.0, 10.0 }
};
osc::Edge cubeEdges[] = {
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
osc::Mesh cubeMesh = {
  8, 12, cubeVertices, cubeEdges
};
Cube cubes[] = {
  Cube(&cubeMesh, 0.03, 1),
  Cube(&cubeMesh, 0.03, 1)
};

osc::Vector3D planeVertices[] = {
  { -45.0, -10.0, 45.0 },
  { -45.0, -10.0, -45.0 },
  { 45.0, -10.0, -45.0 },
  { 45.0, -10.0, 45.0 }
};
osc::Edge planeEdges[] = {
  { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }
};
osc::Mesh planeMesh = {
  4, 4, planeVertices, planeEdges
};
osc::Object plane(&planeMesh);

Scene scene(engine, cubes, 2, &plane, 4);

void setup() {
  randomSeed(millis());
  scene.init();
}

osc::Matrix camera, translation, rotation;
float phase = 0;
void loop() {
  translation.translation(0, 20, 55);
  rotation.rotation(-0.75, phase, 0);
  camera.multiply(translation, rotation);
  
  scene.process(camera);

  phase += 0.0025;
}

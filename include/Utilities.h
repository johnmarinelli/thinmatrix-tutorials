#ifndef Utilities_h
#define Utilities_h

#define _USE_MATH_DEFINES
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>

const int POSITION_ATTRIBUTE_INDEX = 0;
const int NORMAL_ATTRIBUTE_INDEX = 1;
const int TEXTURE_COORD_ATTRIBUTE_INDEX = 2;
const int COLOR_ATTRIBUTE_INDEX = 3;

static double delta = 0.0;
static double dt = 0.0;

const int NUM_ATTRIBUTES = 4;

inline float toRadians(float degrees) {
  return degrees * (M_PI / 180.0f);
}


inline float random(float min, float max) {
  return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

#endif

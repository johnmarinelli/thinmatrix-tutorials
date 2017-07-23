#include "Particle.hpp"
#include "Utilities.h"

Particle::Particle() :
mPosition(glm::vec3{0.0f}),
mVelocity(glm::vec3{0.0f}),
mGravity(1.0f),
mLifespan(1.0f),
mRotationAngle(0.0f),
mScale(1.0f),
mElapsedTime(0.0f) {
}

Particle::Particle(const glm::vec3& pos, const glm::vec3& vel, float gravity, float lifespan, float rotationAngle, float scale) :
  mPosition(pos),
  mVelocity(vel),
  mGravity(gravity),
  mLifespan(lifespan),
  mRotationAngle(rotationAngle),
  mScale(scale),
  mElapsedTime(0.0f) {
}

bool Particle::update(double dt) {
  float dtSeconds = dt * 1000.0f;
  mVelocity.y += GLOBAL_GRAVITY * mGravity * dtSeconds;
  glm::vec3 delta = mVelocity;
  delta *= dtSeconds;
  mPosition += delta;
  
  // dt is in milliseconds, lifespan is in seconds
  mElapsedTime += dtSeconds;
  
  return mElapsedTime < mLifespan;
}

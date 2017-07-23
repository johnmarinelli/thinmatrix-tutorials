#ifndef Particle_hpp
#define Particle_hpp

#include <glm/glm.hpp>

class Particle {
public:
  glm::vec3 mPosition;
  glm::vec3 mVelocity;
  float mGravity;
  float mLifespan;
  float mRotationAngle;
  float mScale;
  
  float mElapsedTime;
  
  Particle();
  Particle(const glm::vec3& pos, const glm::vec3& vel, float gravity, float lifespan, float rotationAngle, float scale);
  
  bool update(double dt);
};

#endif

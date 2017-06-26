#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "TexturedModel.hpp"

class Entity {
public:
  TexturedModel mModel;
  glm::vec3 mPosition;
  glm::vec3 mRotation;
  float mRotationAngle;
  glm::vec3 mScale;
  
  Entity(const TexturedModel& model, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) :
    mModel(model),
    mPosition(pos),
    mRotation(rot),
    mRotationAngle(0.0f),
    mScale(scl) {
    
  }
  
  void increasePosition(const glm::vec3& d) {
    mPosition += d;
  }
  
  void increaseRotation(const glm::vec3& d) {
    mRotation += d;
  }
  
  void increaseScale(const glm::vec3& d) {
    mScale += d;
  }
};

#endif

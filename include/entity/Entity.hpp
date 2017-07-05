#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "TexturedModel.hpp"

class Entity {
public:
  Entity();
  
  Entity(const TexturedModel& model, int texIndex, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl);
  
  void increasePosition(const glm::vec3& d);
  
  void increaseRotation(const glm::vec3& d);
  
  void increaseScale(const glm::vec3& d);
  
  float getTextureXOffset();
  
  float getTextureYOffset();
         
  TexturedModel mModel;
  int mTextureIndex;
  glm::vec3 mPosition;
  glm::vec3 mRotation;
  float mRotationAngle;
  glm::vec3 mScale;
  
};

#endif

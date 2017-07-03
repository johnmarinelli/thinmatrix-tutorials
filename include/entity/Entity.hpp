#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "TexturedModel.hpp"

class Entity {
public:
  TexturedModel mModel;
  int mTextureIndex;
  glm::vec3 mPosition;
  glm::vec3 mRotation;
  float mRotationAngle;
  glm::vec3 mScale;
  
  Entity() :
    mModel(),
    mTextureIndex(0),
    mPosition(glm::vec3{0.0f}),
    mRotation(glm::vec3{0.0f}),
    mRotationAngle(0.0f),
    mScale(glm::vec3{0.0f}) {
  }
  
  Entity(const TexturedModel& model, int texIndex, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) :
    mModel(model),
    mTextureIndex(texIndex),
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
  
  float getTextureXOffset() {
    int numTexRows = mModel.mModelTexture.mNumberOfRows;
    int col = mTextureIndex % numTexRows;
    return (float) col / (float) numTexRows;
  }
  
  float getTextureYOffset() {
    int numTexRows = mModel.mModelTexture.mNumberOfRows;
    int row = mTextureIndex / numTexRows;
    return (float) row / numTexRows;
  }
};

#endif

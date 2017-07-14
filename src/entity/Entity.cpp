#include "Entity.hpp"

Entity::Entity() :
  mModel(),
  mTextureIndex(0),
  mPosition(glm::vec3{0.0f}),
  mRotation(glm::vec3{0.0f}),
  mRotationAngle(0.0f),
  mScale(glm::vec3{0.0f}) {
}

Entity::Entity(const TexturedModel& model, int texIndex, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) :
  mModel(model),
  mTextureIndex(texIndex),
  mPosition(pos),
  mRotation(rot),
  mRotationAngle(0.0f),
  mScale(scl) {
}

float Entity::getTextureXOffset() {
  int numTexRows = mModel.mModelTexture.mNumberOfRows;
  int col = mTextureIndex % numTexRows;
  return (float) col / (float) numTexRows;
}

float Entity::getTextureYOffset() {
  int numTexRows = mModel.mModelTexture.mNumberOfRows;
  int row = mTextureIndex / numTexRows;
  return (float) row / numTexRows;
}

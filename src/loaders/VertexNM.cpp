#include "VertexNM.hpp"

#include <iostream>
#include <memory>

VertexNM::VertexNM() :
  mTextureIndex(NO_INDEX),
  mNormalIndex(NO_INDEX),
  mAverageTangent(glm::vec3{0.0f}) {
}

VertexNM::VertexNM(int idx, const glm::vec3& position) :
  mIndex((GLuint) idx),
  mPosition(position),
  mLength(getLength(position)),
  mTextureIndex(NO_INDEX),
  mNormalIndex(NO_INDEX),
  mAverageTangent(glm::vec3{0.0f}) {
}

float VertexNM::getLength(const glm::vec3& v) {
  return std::sqrt(std::powf(v.x, 2.0f) + std::powf(v.y, 2.0f) + std::powf(v.z, 2.0f));
}

bool VertexNM::isSet() {
  return mTextureIndex != NO_INDEX && mNormalIndex != NO_INDEX;
}

bool VertexNM::hasSameTextureAndNormal(GLuint otherTextureIndex, GLuint otherNormalIndex) {
  return otherTextureIndex == mTextureIndex && otherNormalIndex == mNormalIndex;
}

std::shared_ptr<VertexNM> VertexNM::duplicate(int newIndex) {
  std::shared_ptr<VertexNM> vertex = std::make_shared<VertexNM>(newIndex, mPosition);
  vertex->mTangents.resize(mTangents.size());
  std::copy(mTangents.begin(), mTangents.end(), vertex->mTangents.begin());
  return vertex;
}

glm::vec3 VertexNM::calculateAverageTangent() {
  for (const auto& tan: mTangents) {
    mAverageTangent += tan;
  }
  
  mAverageTangent = glm::normalize(mAverageTangent);
  //std::cout << "Average tangent for vertex at (" << mPosition.x << ", " << mPosition.y << ", " << mPosition.z << "): " << mAverageTangent.x << ", " << mAverageTangent.y << ", " << mAverageTangent.z << '\n';
  return mAverageTangent;
}

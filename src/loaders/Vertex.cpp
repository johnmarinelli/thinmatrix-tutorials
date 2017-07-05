#include "Vertex.hpp"

Vertex::Vertex() :
  mTextureIndex(NO_INDEX),
  mNormalIndex(NO_INDEX) {
}

Vertex::Vertex(int idx, const glm::vec3& position) :
mIndex((GLuint) idx),
mPosition(position),
mLength(getLength(position)),
mTextureIndex(NO_INDEX),
mNormalIndex(NO_INDEX) {
}

float Vertex::getLength(const glm::vec3& v) {
  return std::sqrt(std::powf(v.x, 2.0f) + std::powf(v.y, 2.0f) + std::powf(v.z, 2.0f));
}

bool Vertex::isSet() {
  return mTextureIndex != NO_INDEX && mNormalIndex != NO_INDEX;
}

bool Vertex::hasSameTextureAndNormal(GLuint otherTextureIndex, GLuint otherNormalIndex) {
  return otherTextureIndex == mTextureIndex && otherNormalIndex == mNormalIndex;
}

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>

#include <memory>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <cmath>

class Vertex {
public:
  const static int NO_INDEX = std::numeric_limits<int>::max();
  glm::vec3 mPosition;
  GLuint mTextureIndex;
  GLuint mNormalIndex;
  GLuint mIndex;
  float mLength;
  std::shared_ptr<Vertex> mDuplicate;
  
  Vertex() :
    mTextureIndex(NO_INDEX),
    mNormalIndex(NO_INDEX) {
  }
  
  Vertex(int idx, const glm::vec3& position) :
    mIndex((GLuint) idx),
    mPosition(position),
    mLength(getLength(position)),
    mTextureIndex(NO_INDEX),
    mNormalIndex(NO_INDEX) {
  }
  
  float getLength(const glm::vec3& v) {
    return std::sqrt(std::powf(v.x, 2.0f) + std::powf(v.y, 2.0f) + std::powf(v.z, 2.0f));
  }
  
  bool isSet() {
    return mTextureIndex != NO_INDEX && mNormalIndex != NO_INDEX;
  }
  
  bool hasSameTextureAndNormal(GLuint otherTextureIndex, GLuint otherNormalIndex) {
    return otherTextureIndex == mTextureIndex && otherNormalIndex == mNormalIndex;
  }

};

#endif 

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>

#include <memory>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <cmath>

class Vertex {
public:
  Vertex();
  Vertex(int idx, const glm::vec3& position);
  
  float getLength(const glm::vec3& v);
  bool isSet();
  bool hasSameTextureAndNormal(GLuint otherTextureIndex, GLuint otherNormalIndex);
  
  const static int NO_INDEX = std::numeric_limits<int>::max();
  glm::vec3 mPosition;
  GLuint mTextureIndex;
  GLuint mNormalIndex;
  GLuint mIndex;
  float mLength;
  std::shared_ptr<Vertex> mDuplicate;

};

#endif 

#ifndef VertexNM_hpp
#define VertexNM_hpp

#include <numeric>
#include <memory>
#include <vector>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

class VertexNM {
public:
  VertexNM();
  VertexNM(int idx, const glm::vec3& position);
  
  float getLength(const glm::vec3& v);
  bool isSet();
  bool hasSameTextureAndNormal(GLuint otherTextureIndex, GLuint otherNormalIndex);
  std::shared_ptr<VertexNM> duplicate(int newIndex);
  glm::vec3 calculateAverageTangent();
  
  const static int NO_INDEX = std::numeric_limits<int>::max();
  glm::vec3 mPosition;
  GLuint mTextureIndex;
  GLuint mNormalIndex;
  GLuint mIndex;
  float mLength;
  std::shared_ptr<VertexNM> mDuplicate;
  std::vector<glm::vec3> mTangents;
  glm::vec3 mAverageTangent;

};

#endif

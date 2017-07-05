#ifndef RawModel_hpp
#define RawModel_hpp

#include <vector>
#include <OpenGL/gl3.h>

#include "Utilities.h"

class RawModel {
public:
  GLuint mVaoID;
  GLuint mVertexCount;
  GLuint mIndexCount;
  std::vector<GLuint> mAttributes;
  
  RawModel();
  RawModel(GLuint vaoid, GLuint vtxcount, GLuint idxCount);
};

#endif 

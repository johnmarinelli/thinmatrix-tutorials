#ifndef ModelDataNM_hpp
#define ModelDataNM_hpp

#include <vector>
#include <OpenGL/gl3.h>

class ModelDataNM {
public:
  ModelDataNM();
  
  ModelDataNM(const std::vector<GLfloat>& vertices,
            const std::vector<GLfloat>& textureCoords,
            const std::vector<GLfloat>& normals,
            const std::vector<GLfloat>& tangents,
            const std::vector<GLuint>& indices,
            float furthestPoint);
  
  std::vector<GLfloat> mVertices;
  std::vector<GLfloat> mTextureCoords;
  std::vector<GLfloat> mNormals;
  std::vector<GLfloat> mTangents;
  std::vector<GLuint> mIndices;
  float mFurthestPoint;
  
};

#endif

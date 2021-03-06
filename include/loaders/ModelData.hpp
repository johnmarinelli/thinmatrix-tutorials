#ifndef ModelData_hpp
#define ModelData_hpp

#include <stdio.h>
#include <vector>
#include <OpenGL/gl3.h>

class ModelData {
public:
  ModelData();
  
  ModelData(const std::vector<GLfloat>& vertices,
            const std::vector<GLfloat>& textureCoords,
            const std::vector<GLfloat>& normals,
            const std::vector<GLuint>& indices,
            float furthestPoint);
  
  std::vector<GLfloat> mVertices;
  std::vector<GLfloat> mTextureCoords;
  std::vector<GLfloat> mNormals;
  std::vector<GLuint> mIndices;
  float mFurthestPoint;
  
};

#endif

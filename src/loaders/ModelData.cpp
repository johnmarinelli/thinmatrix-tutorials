#include "ModelData.hpp"

ModelData::ModelData() :
  mFurthestPoint(std::numeric_limits<float>::max()) {
}

ModelData::ModelData(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& textureCoords, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices, float furthestPoint) :
  mVertices(vertices),
  mTextureCoords(textureCoords),
  mNormals(normals),
  mIndices(indices),
  mFurthestPoint(furthestPoint) {
}

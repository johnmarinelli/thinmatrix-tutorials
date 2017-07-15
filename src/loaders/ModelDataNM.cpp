#include "ModelDataNM.hpp"

ModelDataNM::ModelDataNM() :
mFurthestPoint(std::numeric_limits<float>::max()) {
}

ModelDataNM::ModelDataNM(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& textureCoords, const std::vector<GLfloat>& normals, const std::vector<GLfloat>& tangents, const std::vector<GLuint>& indices, float furthestPoint) :
  mVertices(vertices),
  mTextureCoords(textureCoords),
  mNormals(normals),
  mTangents(tangents),
  mIndices(indices),
  mFurthestPoint(furthestPoint) {
}

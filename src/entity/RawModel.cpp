#include "RawModel.hpp"

RawModel::RawModel() :
  mVaoID(std::numeric_limits<int>::max()),
  mVertexCount(0),
  mIndexCount(0) {
}

RawModel::RawModel(GLuint vaoid, GLuint vtxcount, GLuint idxCount) :
  mVaoID(vaoid),
  mVertexCount(vtxcount),
  mIndexCount(idxCount),
  mAttributes({}) {
}

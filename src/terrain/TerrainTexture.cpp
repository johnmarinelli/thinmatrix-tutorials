#include "TerrainTexture.hpp"

TerrainTexture::TerrainTexture() :
  mTextureID(std::numeric_limits<int>::max()) {
}

TerrainTexture::TerrainTexture(GLuint texID) :
  mTextureID(texID) {
}

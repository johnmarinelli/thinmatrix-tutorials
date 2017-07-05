#include "TerrainTexturePack.hpp"

TerrainTexturePack::TerrainTexturePack() {
}

TerrainTexturePack::TerrainTexturePack(const TerrainTexture& bgTex, const TerrainTexture& rTex, const TerrainTexture& gTex, const TerrainTexture& bTex) :
  mBackgroundTexture(bgTex),
  mRTexture(rTex),
  mGTexture(gTex),
  mBTexture(bTex) {
}

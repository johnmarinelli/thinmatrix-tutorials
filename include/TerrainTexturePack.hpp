#ifndef TerrainTexturePack_hpp
#define TerrainTexturePack_hpp

#include <stdio.h>

#include "TerrainTexture.hpp"

class TerrainTexturePack {
public:
  
  TerrainTexture mBackgroundTexture;
  TerrainTexture mRTexture;
  TerrainTexture mGTexture;
  TerrainTexture mBTexture;
  
  TerrainTexturePack() {
  }
  
  TerrainTexturePack(const TerrainTexture& bgTex,
                     const TerrainTexture& rTex,
                     const TerrainTexture& gTex,
                     const TerrainTexture& bTex) :
    mBackgroundTexture(bgTex),
    mRTexture(rTex),
    mGTexture(gTex),
    mBTexture(bTex) {
  }
  
};

#endif

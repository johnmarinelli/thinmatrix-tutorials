#ifndef TerrainTexturePack_hpp
#define TerrainTexturePack_hpp

#include <stdio.h>

#include "TerrainTexture.hpp"

class TerrainTexturePack {
public:
  TerrainTexturePack();
  TerrainTexturePack(const TerrainTexture& bgTex,
                     const TerrainTexture& rTex,
                     const TerrainTexture& gTex,
                     const TerrainTexture& bTex);
  
  TerrainTexture mBackgroundTexture;
  TerrainTexture mRTexture;
  TerrainTexture mGTexture;
  TerrainTexture mBTexture;
  
};

#endif

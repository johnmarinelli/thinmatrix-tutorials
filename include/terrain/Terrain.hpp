#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <png.h>

#include "Loader.hpp"
#include "RawModel.hpp"
#include "TerrainTexturePack.hpp"

class StbImage;

class Terrain {
public:
  const float SIZE = 128.0f;
  const float MAX_HEIGHT = 40.0f;
  const int MAX_PIXEL_VALUE = 256 * 256 * 256;
  
  std::vector<GLfloat> mVertices;
  std::vector<GLfloat> mNormals;
  std::vector<GLuint> mIndices;
  std::vector<GLfloat> mTexCoords;
  
  float mX, mZ;
  
  RawModel mRawModel;
  TerrainTexturePack mTerrainTexturePack;
  TerrainTexture mBlendMap;
  
  struct HeightMap {
    int mWidth, mHeight;
    std::vector<std::vector<float>> mIndexedHeights;
    
    HeightMap() :
      mWidth(0),
      mHeight(0) {
    }
    
    HeightMap(int w, int h) :
      mWidth(w),
      mHeight(h) {
      mIndexedHeights.resize(mHeight);
        
      for (auto i = 0; i < mHeight; ++i) {
        mIndexedHeights[i].resize(mWidth);
      }
    }
    
    void setIndexedHeight(int x, int y, float h) {
      mIndexedHeights[x][y] = h;
    }
  } mHeightMap;
  
  Terrain(int gridX, int gridZ, Loader& loader, const TerrainTexturePack& texturePack, const TerrainTexture& blendMap, const std::string& heightMapPath);
  
  float getHeight(int x, int z, int imgWidth, int imgHeight, const StbImage& img);
  
  glm::vec3 calculateNormal(int x, int z, int imgWidth, int imgHeight, const StbImage& img);
  
  float baryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) const;
  
  void initHeightMap(const StbImage& img);
  
  float getHeightAtCoord(float worldX, float worldZ) const;
  
  RawModel generateTerrain(Loader& loader, const std::string& filepath);
};

#endif

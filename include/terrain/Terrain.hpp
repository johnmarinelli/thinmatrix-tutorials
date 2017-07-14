#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <png.h>

#include "Loader.hpp"
#include "RawModel.hpp"
#include "TerrainTexturePack.hpp"

class Terrain {
public:
  const float SIZE = 800.0f;
  //const int VERTEX_COUNT = 128;
  const float MAX_HEIGHT = 40.0f;
  
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
  
  float getHeight(int x, int z, int imgWidth, int imgHeight, png_bytep* pngRows);
  
  glm::vec3 calculateNormal(int x, int z, int imgWidth, int imgHeight, png_bytep* pngRows);
  
  float baryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) const;
  
  void initHeightMap(png_bytep* pngRows, int width, int height);
  
  float getHeightAtCoord(float worldX, float worldZ) const;
  
  RawModel generateTerrain(Loader& loader, const std::string& filepath);
  
  //png_bytep* readPNG(const std::string& fn, int& width, int& height);
};

#endif

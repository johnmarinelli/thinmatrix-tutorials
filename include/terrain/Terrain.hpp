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
  
  Terrain(int gridX, int gridZ, Loader& loader, const TerrainTexturePack& texturePack, const TerrainTexture& blendMap, const std::string& heightMapPath) :
    mTerrainTexturePack(texturePack),
    mBlendMap(blendMap),
    mX(gridX * SIZE),
    mZ(gridZ * SIZE),
    mHeightMap() {
      mRawModel = generateTerrain(loader, heightMapPath);
  }
  
  float getHeight(int x, int z, int imgWidth, int imgHeight, png_bytep* pngRows) {
    if (x < 0 || x >= imgWidth || z < 0 || z >= imgHeight) {
      return 0.0f;
    }
    
    png_byte* row = pngRows[z];
    
    // NOTE:
    // since heightmap.png produced by thinmatrix tutorial 21
    // is an rgba png,
    // our offset is 4.
    
    // later, you'll want to refactor this to take png_byte `depth` of the png image
    // into consideration.
    unsigned int r = row[x * 4];
    unsigned int g = row[x * 4 + 1];
    unsigned int b = row[x * 4 + 2];
    
    float rr = (r / 255.0f) * 2.0f - 1.0f;
    //float gg = (g / 255.0f) * 2.0f - 1.0f;
    //float bb = (b / 255.0f) * 2.0f - 1.0f;
    
    // since it's a grayscale image we can just use one value
    float height = rr * MAX_HEIGHT;
    return height;
  }
  
  glm::vec3 calculateNormal(int x, int z, int imgWidth, int imgHeight, png_bytep* pngRows) {
    float heightLeft = getHeight(x - 1, z, imgWidth, imgHeight, pngRows);
    float heightRight = getHeight(x + 1, z, imgWidth, imgHeight, pngRows);
    float heightDown = getHeight(x, z - 1, imgWidth, imgHeight, pngRows);
    float heightUp = getHeight(x, z + 1, imgWidth, imgHeight, pngRows);
    
    glm::vec3 normal{heightLeft - heightRight, 2.0f, heightDown - heightUp};
    return glm::normalize(normal);
  }
  
  float baryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) const {
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
  }
  
  void initHeightMap(png_bytep* pngRows, int width, int height) {
    mHeightMap = HeightMap{width, height};
    
    for (auto i = 0; i < height; ++i) {
      for (auto j = 0; j < width; ++j) {
        auto h = getHeight(j, i, width, height, pngRows);
        mHeightMap.setIndexedHeight(j, i, h);
      }
    }
  }
  
  float getHeightAtCoord(float worldX, float worldZ) const {
    float terrainX = worldX - mX;
    float terrainZ = worldZ - mZ;
    float gridSquareSize = SIZE / ((float) mHeightMap.mIndexedHeights.size() - 1);
    float height = 0.0f;
    
    int gridX = std::floor(terrainX / gridSquareSize);
    int gridZ = std::floor(terrainZ / gridSquareSize);
    
    if (gridX >= mHeightMap.mIndexedHeights.size() - 1 || gridZ >= mHeightMap.mIndexedHeights.size() - 1 || gridX < 0 || gridZ < 0) {
      return height;
    }
    
    float xCoord = std::fmod(terrainX, gridSquareSize) / gridSquareSize;
    float zCoord = std::fmod(terrainZ, gridSquareSize) / gridSquareSize;

    if (xCoord <= 1.0f - zCoord) {
      height = baryCentric(glm::vec3{0.0f, mHeightMap.mIndexedHeights.at(gridX).at(gridZ), 0.0f},
                           glm::vec3{1.0f, mHeightMap.mIndexedHeights.at(gridX + 1).at(gridZ), 0.0f},
                           glm::vec3{0.0f, mHeightMap.mIndexedHeights.at(gridX).at(gridZ + 1), 1.0f},
                           glm::vec2{xCoord, zCoord});
    }
    
    else {
      height = baryCentric(glm::vec3{1.0f, mHeightMap.mIndexedHeights.at(gridX + 1).at(gridZ), 0.0f},
                           glm::vec3{1.0f, mHeightMap.mIndexedHeights.at(gridX + 1).at(gridZ + 1), 1.0f},
                           glm::vec3{0.0f, mHeightMap.mIndexedHeights.at(gridX).at(gridZ + 1), 1.0f},
                           glm::vec2{xCoord, zCoord});
    }
    
    return height;
  }
  
  RawModel generateTerrain(Loader& loader, const std::string& filepath) {
    int width, height;
    
    // IMPORTANT:
    // delete pngRows later
    png_bytep* pngRows = readPNG(filepath, width, height);
    initHeightMap(pngRows, width, height);
    
    int VERTEX_COUNT = height;
    
    int count = VERTEX_COUNT * VERTEX_COUNT;

    mVertices.resize(count * 3);
    mNormals.resize(count * 3);
    mTexCoords.resize(count * 2);    
    mIndices.resize(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
    
    int vertexPointer = 0;
    
    for (auto i = 0; i < VERTEX_COUNT; ++i) {
      
      for (auto j = 0; j < VERTEX_COUNT; ++j) {
        float vertexHeight = getHeight(j, i, width, height, pngRows);
        vertexHeight = mHeightMap.mIndexedHeights[j][i];
        
        mVertices[vertexPointer*3] = (float)j/((float)VERTEX_COUNT - 1.0f) * SIZE;
        mVertices[vertexPointer*3+1] = vertexHeight;
        mVertices[vertexPointer*3+2] = (float)i/((float)VERTEX_COUNT - 1.0f) * SIZE;
        
        glm::vec3 normal = calculateNormal(j, i, width, height, pngRows);
        mNormals[vertexPointer*3] = normal.x;
        mNormals[vertexPointer*3+1] = normal.y;
        mNormals[vertexPointer*3+2] = normal.z;
        mTexCoords[vertexPointer*2] = (float)j/((float)VERTEX_COUNT - 1.0f);
        mTexCoords[vertexPointer*2+1] = (float)i/((float)VERTEX_COUNT - 1.0f);
        vertexPointer++;
      }
    }
    
    int pointer = 0;
    
    for (auto gz = 0; gz < VERTEX_COUNT - 1; ++gz) {
      for (auto gx = 0; gx < VERTEX_COUNT - 1; ++gx) {
        int topLeft = (gz*VERTEX_COUNT)+gx;
        int topRight = topLeft + 1;
        int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
        int bottomRight = bottomLeft + 1;
        mIndices[pointer++] = topLeft;
        mIndices[pointer++] = bottomLeft;
        mIndices[pointer++] = topRight;
        mIndices[pointer++] = topRight;
        mIndices[pointer++] = bottomLeft;
        mIndices[pointer++] = bottomRight;
      }
    }
    
    free(pngRows);
  
    return loader.loadToVAO((GLfloat*) &mVertices[0],
                            (GLfloat*) &mNormals[0],
                            (GLfloat*) &mTexCoords[0],
                            (GLuint*) &mIndices[0],
                            (GLuint) mVertices.size(),
                            (GLuint) mNormals.size(),
                            (GLuint) mTexCoords.size(),
                            (GLuint) mIndices.size());
  }
  
  png_bytep* readPNG(const std::string& fn, int& width, int& height) {
    png_byte color_type, bit_depth;
    png_bytep *row_pointers;
    char* filename = (char*) fn.c_str();
    
    FILE *fp = fopen(filename, "rb");
    
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();
    
    png_infop info = png_create_info_struct(png);
    if(!info) abort();
    
    if(setjmp(png_jmpbuf(png))) abort();
    
    png_init_io(png, fp);
    
    png_read_info(png, info);
    
    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);
    
    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt
    
    if(bit_depth == 16)
      png_set_strip_16(png);
    
    if(color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png);
    
    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand_gray_1_2_4_to_8(png);
    
    if(png_get_valid(png, info, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png);
    
    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    
    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png);
    
    png_read_update_info(png, info);
    
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
      row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }
    
    png_read_image(png, row_pointers);
    
    fclose(fp);
    
    return row_pointers;
  }
};

#endif

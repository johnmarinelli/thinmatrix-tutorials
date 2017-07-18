#include "Terrain.hpp"
#include "stb_image.hpp"

Terrain::Terrain(int gridX, int gridZ, Loader& loader, const TerrainTexturePack& texturePack, const TerrainTexture& blendMap, const std::string& heightMapPath) :
  mTerrainTexturePack(texturePack),
  mBlendMap(blendMap),
  mX(gridX * SIZE),
  mZ(gridZ * SIZE),
  mHeightMap() {
  mRawModel = generateTerrain(loader, heightMapPath);
}

float Terrain::getHeight(int x, int z, int imgWidth, int imgHeight, const StbImage& img) {
  if (x < 0 || x >= imgWidth || z < 0 || z >= imgHeight) {
    return 0.0f;
  }
  
  glm::ivec3 rgb = img.getRGB(x, z);
  int h = (rgb.r << 16) + (rgb.g << 8) + rgb.b;
  
  double height = h;
  height /= (MAX_PIXEL_VALUE / 2.0);
  height -= 1.0;
  height *= MAX_HEIGHT;
  return (float) height;
}

glm::vec3 Terrain::calculateNormal(int x, int z, int imgWidth, int imgHeight, const StbImage &img) {
  float heightLeft = getHeight(x - 1, z, imgWidth, imgHeight, img);
  float heightRight = getHeight(x + 1, z, imgWidth, imgHeight, img);
  float heightDown = getHeight(x, z - 1, imgWidth, imgHeight, img);
  float heightUp = getHeight(x, z + 1, imgWidth, imgHeight, img);
  
  glm::vec3 normal{heightLeft - heightRight, 2.0f, heightDown - heightUp};
  return glm::normalize(normal);
}

float Terrain::baryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) const {
  float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
  float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
  float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
  float l3 = 1.0f - l1 - l2;
  return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

void Terrain::initHeightMap(const StbImage& img) {
  int width = img.mWidth;
  int height = img.mHeight;
  mHeightMap = HeightMap{width, height};
  
  for (auto i = 0; i < height; ++i) {
    for (auto j = 0; j < width; ++j) {
      auto h = getHeight(j, i, width, height, img);
      mHeightMap.setIndexedHeight(j, i, h);
    }
  }
}

float Terrain::getHeightAtCoord(float worldX, float worldZ) const {
  float terrainX = worldX - mX;
  float terrainZ = worldZ - mZ;
  float gridSquareSize = SIZE / ((float) mHeightMap.mIndexedHeights.size() - 1);
  float height = std::numeric_limits<float>::min();
  
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

RawModel Terrain::generateTerrain(Loader& loader, const std::string& filepath) {
  StbImage img{filepath.c_str()};
  initHeightMap(img);
  
  int width = img.mWidth;
  int height = img.mHeight;
  
  int VERTEX_COUNT = height;
  
  int count = VERTEX_COUNT * VERTEX_COUNT;
  
  mVertices.resize(count * 3);
  mNormals.resize(count * 3);
  mTexCoords.resize(count * 2);
  mIndices.resize(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
  
  int vertexPointer = 0;
  
  for (auto i = 0; i < VERTEX_COUNT; ++i) {
    
    for (auto j = 0; j < VERTEX_COUNT; ++j) {
      float vertexHeight = getHeight(j, i, width, height, img);
      vertexHeight = mHeightMap.mIndexedHeights[j][i];
      
      mVertices[vertexPointer*3] = (float)j/((float)VERTEX_COUNT - 1.0f) * SIZE;
      mVertices[vertexPointer*3+1] = vertexHeight;
      mVertices[vertexPointer*3+2] = (float)i/((float)VERTEX_COUNT - 1.0f) * SIZE;
      
      glm::vec3 normal = calculateNormal(j, i, width, height, img);
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
  
  return loader.loadToVAO((GLfloat*) &mVertices[0],
                          (GLfloat*) &mNormals[0],
                          (GLfloat*) &mTexCoords[0],
                          (GLuint*) &mIndices[0],
                          (GLuint) mVertices.size(),
                          (GLuint) mNormals.size(),
                          (GLuint) mTexCoords.size(),
                          (GLuint) mIndices.size());
}

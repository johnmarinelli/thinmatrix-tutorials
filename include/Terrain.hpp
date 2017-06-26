#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdio.h>
#include <vector>

#include "Loader.hpp"
#include "RawModel.hpp"
#include "ModelTexture.hpp"

class Terrain {
public:
  const float SIZE = 800.0f;
  const int VERTEX_COUNT = 128;
  
  std::vector<GLfloat> mVertices;
  std::vector<GLfloat> mNormals;
  std::vector<GLuint> mIndices;
  std::vector<GLfloat> mTexCoords;
  
  float mX, mZ;
  
  RawModel mRawModel;
  ModelTexture mModelTexture;
  
  Terrain(int gridX, int gridZ, Loader& loader, const ModelTexture& texture) :
    mModelTexture(texture),
    mX(gridX * SIZE),
    mZ(gridZ * SIZE),
    mRawModel(generateTerrain(loader)) {
  }
  
  RawModel generateTerrain(Loader& loader) {
    int count = VERTEX_COUNT * VERTEX_COUNT;

    mVertices.resize(count * 3);
    mNormals.resize(count * 3);
    mTexCoords.resize(count * 2);    
    mIndices.resize(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
    
    int vertexPointer = 0;
    
    for (auto i = 0; i < VERTEX_COUNT; ++i) {
      for (auto j = 0; j < VERTEX_COUNT; ++j) {
        mVertices[vertexPointer*3] = (float)j/((float)VERTEX_COUNT - 1) * SIZE;
        mVertices[vertexPointer*3+1] = 0;
        mVertices[vertexPointer*3+2] = (float)i/((float)VERTEX_COUNT - 1) * SIZE;
        mNormals[vertexPointer*3] = 0;
        mNormals[vertexPointer*3+1] = 1;
        mNormals[vertexPointer*3+2] = 0;
        mTexCoords[vertexPointer*2] = (float)j/((float)VERTEX_COUNT - 1);
        mTexCoords[vertexPointer*2+1] = (float)i/((float)VERTEX_COUNT - 1);
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
                            (GLuint*) &mIndices[0],
                            (GLfloat*) &mTexCoords[0],
                            (GLfloat*) &mNormals[0],
                            (GLuint) mVertices.size(),
                            (GLuint) mIndices.size(),
                            (GLuint) mTexCoords.size(),
                            (GLuint) mNormals.size());
  }
};

#endif

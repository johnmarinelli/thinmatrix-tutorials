#ifndef TerrainTexture_hpp
#define TerrainTexture_hpp

#include <stdio.h>
#include <limits>
#include <OpenGL/gl3.h>

class TerrainTexture {
public:
  GLuint mTextureID;
  
  TerrainTexture() :
    mTextureID(std::numeric_limits<int>::max()) {
  }
  
  TerrainTexture(GLuint texID) :
    mTextureID(texID) {
    
  }
  
};

#endif

#ifndef TerrainTexture_hpp
#define TerrainTexture_hpp

#include <stdio.h>
#include <limits>
#include <OpenGL/gl3.h>

class TerrainTexture {
public:
  TerrainTexture();
  TerrainTexture(GLuint texID);
  
  GLuint mTextureID;
};

#endif

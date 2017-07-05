#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <png.h>
#include <OpenGL/gl3.h>

class Texture {
public:
  Texture(std::string filename);
  
  GLuint png_texture_load(const char * file_name, int * width, int * height);
  
  GLint mTextureID;
  int mWidth, mHeight;
  
};

#endif

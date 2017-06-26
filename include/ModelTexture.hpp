#ifndef ModelTexture_hpp
#define ModelTexture_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>

class ModelTexture {
public:
  GLint mTextureID;
  float mShineDamper;
  float mReflectivity;
  
  ModelTexture() :
    mTextureID(-1),
    mShineDamper(1.0f),
    mReflectivity(0.0f) {
  }
  
  ModelTexture(GLint texID) :
    mTextureID(texID),
    mShineDamper(1.0f),
    mReflectivity(0.0f) {    
  }
  
};

#endif

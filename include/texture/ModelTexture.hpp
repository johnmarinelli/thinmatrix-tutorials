#ifndef ModelTexture_hpp
#define ModelTexture_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>

class ModelTexture {
public:
  ModelTexture();  
  ModelTexture(GLint texID);
  
  GLint mNormalMapID;
  GLint mTextureID;
  float mShineDamper;
  float mReflectivity;
  int mNumberOfRows;
  bool mHasTransparency;
  bool mUseFakeLighting;
  
};

#endif

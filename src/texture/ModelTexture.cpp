#include "ModelTexture.hpp"

ModelTexture::ModelTexture() :
  mTextureID(-1),
  mShineDamper(1.0f),
  mReflectivity(0.0f),
  mNumberOfRows(1),
  mHasTransparency(false),
  mUseFakeLighting(false) {
}

ModelTexture::ModelTexture(GLint texID) :
  mTextureID(texID),
  mShineDamper(1.0f),
  mReflectivity(0.0f),
  mNumberOfRows(1),
  mHasTransparency(false),
  mUseFakeLighting(false) {
}

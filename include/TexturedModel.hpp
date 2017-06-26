#ifndef TexturedModel_hpp
#define TexturedModel_hpp

#include <stdio.h>

#include "RawModel.hpp"
#include "ModelTexture.hpp"

class TexturedModel {
public:
  RawModel mRawModel;
  ModelTexture mModelTexture;
  
  TexturedModel(RawModel rawModel, ModelTexture modelTexture) :
    mRawModel(rawModel),
    mModelTexture(modelTexture) {
    
  }
};

#endif

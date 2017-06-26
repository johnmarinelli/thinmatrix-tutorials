#ifndef TexturedModel_hpp
#define TexturedModel_hpp

#include <stdio.h>
#include <map>
#include <string>

#include "RawModel.hpp"
#include "ModelTexture.hpp"

enum class TexturedModelType {
  DRAGON,
  STALL,
  CUBE,
  TOTAL_TYPES,
  NONE
};

static std::map<TexturedModelType, std::string> textureModelStrings = {
  std::pair<TexturedModelType, std::string>{TexturedModelType::DRAGON, "DRAGON"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::STALL, "STALL"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::CUBE, "CUBE"}
};

/*static std::map<TexturedModelType, std::string> createTextureModelStringsMap() {
  std::map<TexturedModelType, std::string> map;
}*/

class TexturedModel {
public:
  RawModel mRawModel;
  ModelTexture mModelTexture;
  TexturedModelType mModelType;
  
  TexturedModel() :
    mRawModel(),
    mModelTexture(),
    mModelType(TexturedModelType::NONE) {    
  }
  
  TexturedModel(RawModel rawModel, ModelTexture modelTexture, TexturedModelType type) :
    mRawModel(rawModel),
    mModelTexture(modelTexture),
    mModelType(type) {
  }
};

#endif

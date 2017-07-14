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
  FERN,
  GRASS,
  PLAYER,
  LAMP,
  TOTAL_TYPES,
  NONE
};

static std::map<TexturedModelType, std::string> textureModelStrings = {
  std::pair<TexturedModelType, std::string>{TexturedModelType::DRAGON, "DRAGON"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::STALL, "STALL"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::CUBE, "CUBE"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::FERN, "FERN"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::GRASS, "GRASS"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::PLAYER, "PLAYER"},
  std::pair<TexturedModelType, std::string>{TexturedModelType::LAMP, "LAMP"}
};

class TexturedModel {
public:
  RawModel mRawModel;
  ModelTexture mModelTexture;
  TexturedModelType mModelType;
  
  TexturedModel();
  
  TexturedModel(const RawModel& rawModel, const ModelTexture& modelTexture, TexturedModelType type);
};

#endif

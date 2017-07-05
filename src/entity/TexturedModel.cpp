#include "TexturedModel.hpp"

TexturedModel::TexturedModel() :
  mRawModel(),
  mModelTexture(),
  mModelType(TexturedModelType::NONE) {
}

TexturedModel::TexturedModel(const RawModel& rawModel, const ModelTexture& modelTexture, TexturedModelType type) :
  mRawModel(rawModel),
  mModelTexture(modelTexture),
  mModelType(type) {
}

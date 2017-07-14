#include "NormalShader.hpp"
#include "Utilities.h"
#include "Light.hpp"

NormalShader::NormalShader() :
  ShaderProgram() {
}

void NormalShader::init(const std::string &vertexFilepath, const std::string &fragmentFilepath) {
  initFromFiles(vertexFilepath, fragmentFilepath);
  
  registerUniform("projectionMatrix");
  registerUniform("viewMatrix");
  registerUniform("modelMatrix");
  registerUniform("shineDamper");
  registerUniform("reflectivity");
  registerUniform("ambientFactor");
  registerUniform("skyColor");
  registerUniform("numTextureRows");
  registerUniform("textureAtlasXYOffset");
  
  for (int i = 0; i < MAX_LIGHTS; ++i) {
    registerUniform("lightPosition[" + std::to_string(i) + "]");
    registerUniform("lightColor[" + std::to_string(i) + "]");
    registerUniform("attenuations[" + std::to_string(i) + "]");
  }
  
}

void NormalShader::loadTextureAtlasXYOffset(const glm::vec2& offset) {
  auto location = mUniforms["textureAtlasXYOffset"];
  
  glUniform2f(location, offset.x, offset.y);
}

void NormalShader::loadNumTextureRows(float num) {
  auto numTextureRowsLocation = mUniforms["numTextureRows"];
  
  glUniform1f(numTextureRowsLocation, num);
}

void NormalShader::loadSkyColor(const glm::vec3& skyColor) {
  auto skyColorLocation = mUniforms["skyColor"];
  
  glUniform3f(skyColorLocation, skyColor.r, skyColor.g, skyColor.b);
}

void NormalShader::loadLights(const std::vector<Light>& lights) {  
  for (int i = 0; i < MAX_LIGHTS; ++i) {
    auto iStr = std::to_string(i);
    auto lightPosLocation = mUniforms["lightPosition[" + iStr + "]"];
    auto lightColLocation = mUniforms["lightColor[" + iStr + "]"];
    auto attLocation = mUniforms["attenuations[" + iStr + "]"];
    if (i < lights.size()) {
      auto light = lights.at(i);
      
      glUniform3f(lightPosLocation, light.mPosition.x, light.mPosition.y, light.mPosition.z);
      glUniform3f(lightColLocation, light.mColor.r, light.mColor.g, light.mColor.b);
      glUniform3f(attLocation, light.mAttenuation.x, light.mAttenuation.y, light.mAttenuation.z);
    }
    else {
      glUniform3f(lightPosLocation, 0.0f, 0.0f, 0.0f);
      glUniform3f(lightColLocation, 0.0f, 0.0f, 0.0f);
      glUniform3f(attLocation, 1.0f, 0.0f, 0.0f);
    }
  }
}

void NormalShader::loadShineVariables(float shineDamper, float reflectivity) {
  auto shineDamperLocation = mUniforms["shineDamper"],
  reflectivityLocation = mUniforms["reflectivity"];
  
  glUniform1f(shineDamperLocation, shineDamper);
  glUniform1f(reflectivityLocation, reflectivity);
}
void NormalShader::loadAmbientFactor(float ambientFactor) {
  auto ambientFactorLocation = mUniforms["ambientFactor"];
  
  glUniform1f(ambientFactorLocation, ambientFactor);
}

void NormalShader::loadProjectionMatrix(const glm::mat4& projectionMatrix) {
  auto projectionMatrixLocation = mUniforms["projectionMatrix"];
  
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void NormalShader::loadViewMatrix(const glm::mat4& viewMatrix) {
  auto viewMatrixLocation = mUniforms["viewMatrix"];
  
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void NormalShader::loadModelMatrix(const glm::mat4& modelMatrix) {
  auto modelMatrixLocation = mUniforms["modelMatrix"];
  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

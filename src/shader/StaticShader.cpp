#include "StaticShader.hpp"


StaticShader::StaticShader() :
  ShaderProgram() {
}

void StaticShader::loadTextureAtlasXYOffset(const glm::vec2& offset, const std::string& textureAtlasXYOffsetName) {
  auto location = mUniforms[textureAtlasXYOffsetName];
  
  glUniform2f(location, offset.x, offset.y);
}

void StaticShader::loadNumTextureRows(float num, const std::string& numTextureRowsName) {
  auto numTextureRowsLocation = mUniforms[numTextureRowsName];
  
  glUniform1f(numTextureRowsLocation, num);
}

void StaticShader::loadUseFakeLighting(bool useFake, const std::string& useFakeLightingName) {
  auto useFakeLightingLocation = mUniforms[useFakeLightingName];
  
  glUniform1f(useFakeLightingLocation, useFake ? 1.0 : 0.0);
}

void StaticShader::loadSkyColor(const glm::vec3& skyColor, const std::string& skyColorName) {
  auto skyColorLocation = mUniforms[skyColorName];
  
  glUniform3f(skyColorLocation, skyColor.r, skyColor.g, skyColor.b);
}

void StaticShader::loadLight(const Light& light, const std::string& posName, const std::string& colName) {
  auto lightPosLocation = mUniforms[posName],
  lightColLocation = mUniforms[colName];
  
  glUniform3f(lightPosLocation, light.mPosition.x, light.mPosition.y, light.mPosition.z);
  glUniform3f(lightColLocation, light.mColor.r, light.mColor.g, light.mColor.b);
}

void StaticShader::loadShineVariables(float shineDamper, float reflectivity, const std::string& damperName, const std::string& refName) {
  auto shineDamperLocation = mUniforms[damperName],
  reflectivityLocation = mUniforms[refName];
  
  glUniform1f(shineDamperLocation, shineDamper);
  glUniform1f(reflectivityLocation, reflectivity);
}
void StaticShader::loadAmbientFactor(float ambientFactor, const std::string& ambientName) {
  auto ambientFactorLocation = mUniforms[ambientName];
  
  glUniform1f(ambientFactorLocation, ambientFactor);
}

void StaticShader::loadProjectionMatrix(const glm::mat4& projectionMatrix, const std::string& projectionMatrixName) {
  auto projectionMatrixLocation = mUniforms[projectionMatrixName];
  
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void StaticShader::loadViewMatrix(const glm::mat4& viewMatrix, const std::string& viewMatrixName) {
  auto viewMatrixLocation = mUniforms[viewMatrixName];
  
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void StaticShader::loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName) {
  auto modelMatrixLocation = mUniforms[modelMatrixName];
  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

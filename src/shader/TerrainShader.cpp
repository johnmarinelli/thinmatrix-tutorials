#include "TerrainShader.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

TerrainShader::TerrainShader() :
  ShaderProgram() {
}

void TerrainShader::connectTextureUnits() {
  loadInt(0, "backgroundTexture");
  loadInt(1, "rTexture");
  loadInt(2, "gTexture");
  loadInt(3, "bTexture");
  loadInt(4, "blendMap");
}

void TerrainShader::loadInt(GLuint i, const std::string& name) {
  auto location = mUniforms[name];
  
  glUniform1i(location, i);
}

void TerrainShader::loadFloat(GLfloat f, const std::string& name) {
  auto location = mUniforms[name];
  
  glUniform1f(location, f);
}

void TerrainShader::loadLight(const Light& light, const std::string& posName, const std::string& colName) {
  auto lightPosLocation = mUniforms[posName],
  lightColLocation = mUniforms[colName];
  
  glUniform3f(lightPosLocation, light.mPosition.x, light.mPosition.y, light.mPosition.z);
  glUniform3f(lightColLocation, light.mColor.r, light.mColor.g, light.mColor.b);
}

void TerrainShader::loadSkyColor(const glm::vec3& skyColor, const std::string& skyColorName) {
  auto skyColorLocation = mUniforms[skyColorName];
  
  glUniform3f(skyColorLocation, skyColor.r, skyColor.g, skyColor.b);
}

void TerrainShader::loadShineVariables(float shineDamper, float reflectivity, const std::string& damperName, const std::string& refName) {
  auto shineDamperLocation = mUniforms[damperName],
  reflectivityLocation = mUniforms[refName];
  
  glUniform1f(shineDamperLocation, shineDamper);
  glUniform1f(reflectivityLocation, reflectivity);
}
void TerrainShader::loadAmbientFactor(float ambientFactor, const std::string& ambientName) {
  auto ambientFactorLocation = mUniforms[ambientName];
  
  glUniform1f(ambientFactorLocation, ambientFactor);
}

void TerrainShader::loadProjectionMatrix(const glm::mat4& projectionMatrix, const std::string& projectionMatrixName) {
  auto projectionMatrixLocation = mUniforms[projectionMatrixName];
  
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void TerrainShader::loadViewMatrix(const glm::mat4& viewMatrix, const std::string& viewMatrixName) {
  auto viewMatrixLocation = mUniforms[viewMatrixName];
  
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void TerrainShader::loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName) {
  auto modelMatrixLocation = mUniforms[modelMatrixName];
  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
  
}
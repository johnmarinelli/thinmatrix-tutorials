#include "WaterShader.hpp"

void WaterShader::init(const std::string& vertexFilepath, const std::string& fragmentFilepath) {
  initFromFiles(vertexFilepath, fragmentFilepath);
  
  registerUniform("modelMatrix");
  registerUniform("projectionMatrix");
  registerUniform("viewMatrix");
  registerUniform("reflectionTexture");
  registerUniform("refractionTexture");
  registerUniform("dudvMap");
  registerUniform("moveFactor");
  registerUniform("cameraPosition");
}

void WaterShader::connectTextureUnits() {
  auto reflectionLocation = mUniforms["reflectionTexture"];
  auto refractionLocation = mUniforms["refractionTexture"];
  auto dudvLocation = mUniforms["dudvMap"];
  glUniform1i(reflectionLocation, 0);
  glUniform1i(refractionLocation, 1);
  glUniform1i(dudvLocation, 2);
}

void WaterShader::loadMoveFactor(float moveFactor) {
  auto location = mUniforms["moveFactor"];
  glUniform1f(location, moveFactor);
}

void WaterShader::loadCameraPosition(const glm::vec3 &cameraPosition) {
  auto location = mUniforms["cameraPosition"];
  glUniform3fv(location, 1, glm::value_ptr(cameraPosition));
}

void WaterShader::loadProjectionMatrix(const glm::mat4& projectionMatrix) {
  auto projectionMatrixLocation = mUniforms["projectionMatrix"];
  
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void WaterShader::loadViewMatrix(const glm::mat4& viewMatrix) {
  auto viewMatrixLocation = mUniforms["viewMatrix"];
  
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void WaterShader::loadModelMatrix(const glm::mat4& modelMatrix) {
  auto modelMatrixLocation = mUniforms["modelMatrix"];
  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

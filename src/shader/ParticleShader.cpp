#include "ParticleShader.hpp"

void ParticleShader::init(const std::string &vertexFilepath, const std::string &fragmentFilepath) {
  initFromFiles(vertexFilepath, fragmentFilepath);
  
  registerUniform("modelViewMatrix");
  registerUniform("projectionMatrix");
}

void ParticleShader::loadProjectionMatrix(const glm::mat4 &projectionMatrix) {
  auto location = mUniforms["projectionMatrix"];
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void ParticleShader::loadModelViewMatrix(const glm::mat4 &modelViewMatrix) {
  auto location = mUniforms["modelViewMatrix"];
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

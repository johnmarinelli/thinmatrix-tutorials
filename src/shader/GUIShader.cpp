#include "GUIShader.hpp"

void GUIShader::init(const std::string& vertexFilepath, const std::string& fragmentFilepath) {
  initFromFiles(vertexFilepath, fragmentFilepath);
  registerUniform("modelMatrix");
}

void GUIShader::loadModelMatrix(const glm::mat4& modelMatrix) {
  auto modelMatrixLocation = mUniforms["modelMatrix"];
  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

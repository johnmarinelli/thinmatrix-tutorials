#include "GUIShader.hpp"

void GUIShader::loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName) {
  auto modelMatrixLocation = mUniforms[modelMatrixName];
  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

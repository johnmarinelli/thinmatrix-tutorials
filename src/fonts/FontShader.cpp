#include "FontShader.hpp"

void FontShader::init(const std::string& vertexFilepath, const std::string& fragmentFilepath) {

  initFromFiles(vertexFilepath, fragmentFilepath);

  registerUniform("tex");
  registerUniform("col");
  registerUniform("projectionMatrix");
  
  GLuint texLocation = mUniforms["tex"];
  glUniform1i(texLocation, 0);
  
  glm::vec3 color{1.0f, 1.0f, 1.0f};
  GLuint colorLocation = mUniforms["col"];
  glUniform3fv(colorLocation, 1, glm::value_ptr(color));

}

void FontShader::loadProjectionMatrix(const glm::mat4& projectionMatrix) {
  auto projectionMatrixLocation = mUniforms["projectionMatrix"];
  
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

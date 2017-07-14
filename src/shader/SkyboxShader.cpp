#include "SkyboxShader.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "Utilities.h"

SkyboxShader::SkyboxShader() :
  ShaderProgram() {
}

void SkyboxShader::init(const std::string& vertexFilepath, const std::string& fragmentFilepath) {
  initFromFiles(vertexFilepath, fragmentFilepath);
  mRotation = 0.0f;
  
  registerUniform("fogColor");
  registerUniform("dayCubeMap");
  registerUniform("nightCubeMap");
  registerUniform("blendFactor");
  registerUniform("projectionMatrix");
  registerUniform("viewMatrix");
  
  auto dayCubeMapLocation = mUniforms["dayCubeMap"];
  glUniform1i(dayCubeMapLocation, 0);
  
  auto nightCubeMapLocation = mUniforms["nightCubeMap"];
  glUniform1i(nightCubeMapLocation, 1);
}

void SkyboxShader::loadBlendFactor(float bf) {
  auto location = mUniforms["blendFactor"];
  
  glUniform1f(location, bf);
}

void SkyboxShader::loadFogColor(const glm::vec3& fogColor) {
  auto location = mUniforms["fogColor"];
  glUniform3fv(location, 1, glm::value_ptr(fogColor));
}

void SkyboxShader::loadProjectionMatrix(const glm::mat4 &projectionMatrix) {
  auto location = mUniforms["projectionMatrix"];
  
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void SkyboxShader::update(double dt) {
  mRotation += SKYBOX_ROTATION_SPEED * (float) dt;
}

void SkyboxShader::loadViewMatrix(const glm::mat4 &viewMatrix) {
  auto location = mUniforms["viewMatrix"];
  glm::mat4 skyboxViewMatrix = viewMatrix;
  // to prevent skybox from translating with the camera,
  // set the last column to 0.0f
  skyboxViewMatrix[3][0] = 0.0f;
  skyboxViewMatrix[3][1] = 0.0f;
  skyboxViewMatrix[3][2] = 0.0f;
  skyboxViewMatrix = glm::rotate(skyboxViewMatrix, mRotation, glm::vec3{0.0f, 1.0f, 0.0f});

  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(skyboxViewMatrix));
}

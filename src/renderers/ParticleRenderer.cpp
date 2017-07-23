#include "ParticleRenderer.hpp"
#include "Loader.hpp"
#include "Camera.hpp"

ParticleRenderer::ParticleRenderer() {

}

void ParticleRenderer::init(Loader &loader, const glm::mat4 &projectionMatrix, Camera &camera) {
  mProjectionMatrix = projectionMatrix;
  mCameraHdl = &camera;
  
  GLfloat positions[8]{
    -1.0f, 1.0f,
    -1.0f, -1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f
  };
  
  mQuad = loader.loadQuadToVAO(positions);
  mShaderProgram.init("resources/shaders/particle.vert", "resources/shaders/particle.frag");
  mShaderProgram.use();
  mShaderProgram.loadProjectionMatrix(projectionMatrix);
  mShaderProgram.disable();
}

void ParticleRenderer::render(const std::vector<Particle> &particles) {
  glm::mat4 viewMatrix = mCameraHdl->createViewMatrix();
  prepare();
  for (const auto& p : particles) {
    updateModelViewMatrix(p.mPosition, p.mRotationAngle, p.mScale, viewMatrix);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mQuad.mVertexCount);
  }
  finishRendering();
}

void ParticleRenderer::prepare() {
  mShaderProgram.use();
  glBindVertexArray(mQuad.mVaoID);
  glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // prevents particles from being rendered to depth buffer
  glDepthMask(false);
}

void ParticleRenderer::updateModelViewMatrix(const glm::vec3& position, float rotation, float scale, glm::mat4& viewMatrix) {
  glm::mat4 modelMatrix = glm::mat4{1.0f};
  modelMatrix = glm::translate(modelMatrix, position);
  
  // make the rotation portion of model matrix
  // the inverse of the view matrix
  // so that there is no rotation.
  modelMatrix[0][0] = viewMatrix[0][0];
  modelMatrix[0][1] = viewMatrix[1][0];
  modelMatrix[0][2] = viewMatrix[2][0];
  
  modelMatrix[1][0] = viewMatrix[0][1];
  modelMatrix[1][1] = viewMatrix[1][1];
  modelMatrix[1][2] = viewMatrix[2][1];
  
  modelMatrix[2][0] = viewMatrix[0][2];
  modelMatrix[2][1] = viewMatrix[1][2];
  modelMatrix[2][2] = viewMatrix[2][2];
  
  // now rotate along camera's z axis
  modelMatrix = glm::rotate(modelMatrix, toRadians(rotation), glm::vec3{0.0f, 0.0f, 1.0f});
  
  modelMatrix = glm::scale(modelMatrix, glm::vec3{scale, scale, scale});
  
  glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
  
  mShaderProgram.loadModelViewMatrix(modelViewMatrix);
}

void ParticleRenderer::finishRendering() {
  glDepthMask(true);
  glDisable(GL_BLEND);
  glDisableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
  glBindVertexArray(0);
  
  mShaderProgram.disable();
}

void ParticleRenderer::cleanUp() {
  mShaderProgram.cleanUp();
}

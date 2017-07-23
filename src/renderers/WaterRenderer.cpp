#include "WaterRenderer.hpp"
#include "Camera.hpp"

WaterRenderer::WaterRenderer() :
  mShaderProgram(),
  mCameraHdl(nullptr) {
}

void WaterRenderer::init(Loader& loader, const glm::mat4& projMatrix, Camera& camera, WaterFrameBuffers& waterFBs) {
  mDuDvTexture = loader.loadTexture("resources/textures/water/waterDUDV.png");
  mNormalMap = loader.loadTexture("resources/textures/water/normalMap.png");
  mProjectionMatrix = projMatrix;
  mCameraHdl = &camera;
  mWaterFramebuffersHdl = &waterFBs;
  mMoveFactor = 0.0f;
  
  mShaderProgram.use();
  mShaderProgram.loadProjectionMatrix(mProjectionMatrix);
  mShaderProgram.connectTextureUnits();
  mShaderProgram.disable();
}

void WaterRenderer::loadLight(const Light& light) {
  mShaderProgram.use();
  mShaderProgram.loadLight(light);
  mShaderProgram.disable();
}

void WaterRenderer::update(double dt) {
  mMoveFactor += WAVE_SPEED * dt;
  mMoveFactor = fmodf(mMoveFactor, 1.0f);
}

void WaterRenderer::render(const std::vector<WaterTile>& tiles) {
  prepare();
  for (const auto tile : tiles) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, glm::vec3{tile.mX, tile.mHeight, tile.mZ});
    modelMatrix = glm::scale(modelMatrix, glm::vec3{tile.TILE_SIZE});
    mShaderProgram.loadModelMatrix(modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, mQuad.mVertexCount);
  }
  unbind();
}

void WaterRenderer::prepare() {
  mShaderProgram.use();
  mShaderProgram.loadCameraPosition(mCameraHdl->mPosition);
  mShaderProgram.loadViewMatrix(mCameraHdl->createViewMatrix());
  mShaderProgram.loadMoveFactor(mMoveFactor);
  glBindVertexArray(mQuad.mVaoID);
  
  for (auto att : mQuad.mAttributes) {
    glEnableVertexAttribArray(att);
  }
  
  // activate & bind reflection texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mWaterFramebuffersHdl->mReflectionTextureID);
  
  // activate & bind refraction texture
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mWaterFramebuffersHdl->mRefractionTextureID);
  
  // activate & bind dudv texture
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, mDuDvTexture);
  
  // activate & bind normal map
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, mNormalMap);
  
  // activate & bind refraction depth texture
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, mWaterFramebuffersHdl->mRefractionDepthBufferID);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind() {
  glDisable(GL_BLEND);
  for (auto att : mQuad.mAttributes) {
    glDisableVertexAttribArray(att);
  }
  glBindVertexArray(0);
  mShaderProgram.disable();
}

void WaterRenderer::setupVAO(Loader& loader) {
  float vertices[] = {
    -1, -1,
    -1, 1,
    1, -1,
    1, -1,
    -1, 1,
     1, 1
  };
  mQuad = loader.loadWaterToVAO(vertices);
}

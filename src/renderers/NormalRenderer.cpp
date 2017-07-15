#include "NormalRenderer.hpp"

NormalRenderer::NormalRenderer() {
  
}

void NormalRenderer::init(const glm::mat4 &projMatrix, Camera &camera) {
  mProjectionMatrix = projMatrix;
  mCameraHdl = &camera;
  
  mShaderProgram.use();
  mShaderProgram.loadProjectionMatrix(mProjectionMatrix);
  mShaderProgram.connectTextureUnits();
  mShaderProgram.disable();

}

void NormalRenderer::render(const TexturedEntityMap &entities, const std::vector<Light> &lights, const glm::vec3& skyColor) {
  mShaderProgram.use();
  prepare(lights, skyColor);
  for (auto kv : entities) {
    auto pair = kv.second;
    auto texturedModel = pair.first;
    auto entitiesBatch = pair.second;
    prepareTexturedModel(texturedModel);
    for (auto entity : entitiesBatch) {
      prepareInstance(entity);
      glDrawElements(GL_TRIANGLES, texturedModel.mRawModel.mIndexCount, GL_UNSIGNED_INT, 0);
    }
    unbindTexturedModel(texturedModel);
  }
  mShaderProgram.disable();
}

void NormalRenderer::prepareTexturedModel(const TexturedModel &model) {
  auto rawModel = model.mRawModel;
  auto texture = model.mModelTexture;
  
  glBindVertexArray(rawModel.mVaoID);
  
  for (auto att : rawModel.mAttributes) {
    glEnableVertexAttribArray(att);
  }
  
  if (texture.mHasTransparency) {
    glDisable(GL_CULL_FACE);
  }
  
  mShaderProgram.loadShineVariables(texture.mShineDamper, texture.mReflectivity);
  mShaderProgram.loadNumTextureRows(texture.mNumberOfRows);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, model.mModelTexture.mTextureID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, model.mModelTexture.mNormalMapID);
}

void NormalRenderer::unbindTexturedModel(const TexturedModel &model) {
  glEnable(GL_CULL_FACE);
  auto rawModel = model.mRawModel;
  
  for (auto att : rawModel.mAttributes) {
    glDisableVertexAttribArray(att);
  }
  glBindVertexArray(0);
}

void NormalRenderer::prepareInstance(std::shared_ptr<Entity> entity) {
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, entity->mPosition);
  modelMatrix = glm::rotate(modelMatrix, toRadians(entity->mRotationAngle), entity->mRotation);
  modelMatrix = glm::scale(modelMatrix, entity->mScale);
  mShaderProgram.loadModelMatrix(modelMatrix);
  mShaderProgram.loadTextureAtlasXYOffset(glm::vec2{entity->getTextureXOffset(), entity->getTextureYOffset()});
}

void NormalRenderer::prepare(const std::vector<Light> &lights, const glm::vec3 &skyColor) {
  mShaderProgram.loadSkyColor(skyColor);
  
  auto viewMatrix = mCameraHdl->createViewMatrix();
  mShaderProgram.loadLights(lights, viewMatrix);
  mShaderProgram.loadViewMatrix(viewMatrix);
}

void NormalRenderer::cleanUp() {
  mShaderProgram.cleanUp();
}


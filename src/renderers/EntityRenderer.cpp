#include "EntityRenderer.hpp"

EntityRenderer::EntityRenderer() {
}

void EntityRenderer::init(const glm::mat4& projMatrix) {
  mProjectionMatrix = projMatrix;
  mShaderProgram.use();
  mShaderProgram.loadProjectionMatrix(mProjectionMatrix);
  mShaderProgram.disable();
}

void EntityRenderer::render(const TexturedEntityMap& entities) {
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
    glBindVertexArray(0);
  }
}

void EntityRenderer::prepareTexturedModel(const TexturedModel& model) {
  auto rawModel = model.mRawModel;
  auto texture = model.mModelTexture;
  
  glBindVertexArray(rawModel.mVaoID);
  
  for (auto att : rawModel.mAttributes) {
    glEnableVertexAttribArray(att);
  }
  
  if (texture.mHasTransparency) {
    disableCulling();
  }
  
  mShaderProgram.loadShineVariables(texture.mShineDamper, texture.mReflectivity);
  mShaderProgram.loadUseFakeLighting(texture.mUseFakeLighting);
  mShaderProgram.loadNumTextureRows(texture.mNumberOfRows);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, model.mModelTexture.mTextureID);
}

void EntityRenderer::unbindTexturedModel(const TexturedModel& model) {
  enableCulling();
  auto rawModel = model.mRawModel;
  for (auto att : rawModel.mAttributes) {
    glDisableVertexAttribArray(att);
  }
  glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(std::shared_ptr<Entity> entity) {
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, entity->mPosition);
  modelMatrix = glm::rotate(modelMatrix, toRadians(entity->mRotationAngle), entity->mRotation);
  modelMatrix = glm::scale(modelMatrix, entity->mScale);
  mShaderProgram.loadModelMatrix(modelMatrix);
  mShaderProgram.loadTextureAtlasXYOffset(glm::vec2{entity->getTextureXOffset(), entity->getTextureYOffset()});
}

void EntityRenderer::enableCulling() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void EntityRenderer::disableCulling() {
  glDisable(GL_CULL_FACE);
}

void EntityRenderer::cleanUp() {
  mShaderProgram.cleanUp();
}

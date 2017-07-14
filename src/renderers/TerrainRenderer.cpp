#include "TerrainRenderer.hpp"

#include <OpenGL/gl3.h>
#include <OpenGl/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity.hpp"
#include "ShaderProgram.hpp"
#include "Utilities.h"
#include "Camera.hpp"
#include "Terrain.hpp"

TerrainRenderer::TerrainRenderer() {
}

void TerrainRenderer::init(const glm::mat4& projMatrix) {
  mProjectionMatrix = projMatrix;
  mShaderProgram.use();
  mShaderProgram.connectTextureUnits();
  mShaderProgram.disable();
}

void TerrainRenderer::render(const std::vector<Terrain>& terrains) {
  for (auto terrain : terrains) {
    prepareTerrain(terrain);
    prepareInstance(terrain);
    glDrawElements(GL_TRIANGLES, terrain.mRawModel.mIndexCount, GL_UNSIGNED_INT, 0);
    unbindTexturedModel(terrain);
    glBindVertexArray(0);
  }
}

void TerrainRenderer::prepareTerrain(const Terrain& model) {
  auto rawModel = model.mRawModel;
  
  glBindVertexArray(rawModel.mVaoID);
  
  for (auto att : rawModel.mAttributes) {
    glEnableVertexAttribArray(att);
  }
  
  bindTextures(model);
  mShaderProgram.loadShineVariables(1.0f, 0.0f);
}

void TerrainRenderer::bindTextures(const Terrain& terrain) {
  auto pack = terrain.mTerrainTexturePack;
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, pack.mBackgroundTexture.mTextureID);
  
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, pack.mRTexture.mTextureID);
  
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, pack.mGTexture.mTextureID);
  
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, pack.mBTexture.mTextureID);
  
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, terrain.mBlendMap.mTextureID);
}

void TerrainRenderer::unbindTexturedModel(const Terrain& terrain) {
  auto rawModel = terrain.mRawModel;
  for (auto att : rawModel.mAttributes) {
    glDisableVertexAttribArray(att);
  }
  glBindVertexArray(0);
}

void TerrainRenderer::prepareInstance(const Terrain& terrain) {
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::scale(modelMatrix, glm::vec3{1.0f});
  modelMatrix = glm::translate(modelMatrix, glm::vec3{terrain.mX, 0.0f, terrain.mZ});
  mShaderProgram.loadModelMatrix(modelMatrix);
}

void TerrainRenderer::cleanUp() {
  mShaderProgram.cleanUp();
}















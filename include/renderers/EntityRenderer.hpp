#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <OpenGl/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Entity.hpp"
#include "StaticShader.hpp"
#include "Utilities.h"
#include "Camera.hpp"

class EntityRenderer {
public:
  GLFWwindow* mWindowHdl;
  StaticShader mShaderProgram;
  Camera mCamera;
  glm::mat4 mProjectionMatrix;
  
  typedef std::map<TexturedModelType, std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>> TexturedEntityMap;
  
  EntityRenderer() :
    mWindowHdl(nullptr) {
  }
  
  EntityRenderer(GLFWwindow* window, const StaticShader& program) :
    mWindowHdl(window),
    mShaderProgram(program) {
  }

  void init(const glm::mat4 projMatrix) {
    mProjectionMatrix = projMatrix;
    mShaderProgram.use();
    mShaderProgram.loadProjectionMatrix(mProjectionMatrix, "projectionMatrix");
    mShaderProgram.disable();
  }
  
  void render(const TexturedEntityMap& entities) {
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
  
  void prepareTexturedModel(const TexturedModel& model) {
    auto rawModel = model.mRawModel;
    auto texture = model.mModelTexture;
    
    glBindVertexArray(rawModel.mVaoID);
    
    for (auto att : rawModel.mAttributes) {
      glEnableVertexAttribArray(att);
    }
    
    if (texture.mHasTransparency) {
      disableCulling();
    }
    
    mShaderProgram.loadShineVariables(texture.mShineDamper, texture.mReflectivity, "shineDamper", "reflectivity");
    mShaderProgram.loadUseFakeLighting(texture.mUseFakeLighting, "useFakeLighting");
    mShaderProgram.loadNumTextureRows(texture.mNumberOfRows, "numTextureRows");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.mModelTexture.mTextureID);
  }
  
  void unbindTexturedModel(const TexturedModel& model) {
    enableCulling();
    auto rawModel = model.mRawModel;
    for (auto att : rawModel.mAttributes) {
      glDisableVertexAttribArray(att);
    }
    glBindVertexArray(0);
  }
  
  void prepareInstance(std::shared_ptr<Entity> entity) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, entity->mPosition);
    modelMatrix = glm::rotate(modelMatrix, toRadians(entity->mRotationAngle), entity->mRotation);
    modelMatrix = glm::scale(modelMatrix, entity->mScale);
    mShaderProgram.loadModelMatrix(modelMatrix, "modelMatrix");
    mShaderProgram.loadTextureAtlasXYOffset(glm::vec2{entity->getTextureXOffset(), entity->getTextureYOffset()}, "textureAtlasXYOffset");
  }
  
  void enableCulling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
  
  void disableCulling() {
    glDisable(GL_CULL_FACE);
  }
};

#endif

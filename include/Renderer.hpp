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
#include "ShaderProgram.hpp"
#include "Utilities.h"
#include "Camera.hpp"

class Renderer {
public:
  glm::mat4 mProjectionMatrix;
  GLFWwindow* mWindowHdl;
  ShaderProgram mShaderProgram;
  int mRenderWidth, mRenderHeight;
  Camera mCamera;
  
  typedef std::map<TexturedModelType, std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>> TexturedEntityMap;
  
  Renderer() :
    mWindowHdl(nullptr),
    mRenderWidth(0),
    mRenderHeight(0) {
  }
  
  Renderer(GLFWwindow* window) :
    mWindowHdl(window),
    mRenderWidth(0),
    mRenderHeight(0) {
  }
  
  Renderer(GLFWwindow* window, const ShaderProgram& program) :
    mWindowHdl(window),
    mRenderWidth(0),
    mRenderHeight(0),
    mShaderProgram(program) {
  }
  
  void init() {
    glfwGetFramebufferSize(mWindowHdl, &mRenderWidth, &mRenderHeight);
    mProjectionMatrix = createProjectionMatrix(mRenderWidth, mRenderHeight);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
  
  void prepare() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderProgram.loadProjectionMatrix(mProjectionMatrix, "projectionMatrix");
  }
  
  glm::mat4 createProjectionMatrix(int width, int height) const {
    float ratio;
    ratio = width / (float) height;
    
    glm::mat4 projection;
    
    //projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    projection = glm::perspective(70.0f, ratio, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
    
    return projection;
  }
  
  void moveCamera(MovementDirection dir) {
    mCamera.move(dir);
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
    }
  }
  
  void prepareTexturedModel(const TexturedModel& model) {
    auto rawModel = model.mRawModel;
    auto texture = model.mModelTexture;
    
    glBindVertexArray(rawModel.mVaoID);
    
    for (auto att : rawModel.mAttributes) {
      glEnableVertexAttribArray(att);
    }
    
    mShaderProgram.loadShineVariables(texture.mShineDamper, texture.mReflectivity, "shineDamper", "reflectivity");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.mModelTexture.mTextureID);
  }
  
  void unbindTexturedModel(const TexturedModel& model) {
    auto rawModel = model.mRawModel;
    for (auto att : rawModel.mAttributes) {
      glDisableVertexAttribArray(att);
    }
  }
  
  void prepareInstance(std::shared_ptr<Entity> entity) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::scale(modelMatrix, entity->mScale);
    modelMatrix = glm::rotate(modelMatrix, entity->mRotationAngle, entity->mRotation);
    modelMatrix = glm::translate(modelMatrix, entity->mPosition);
    mShaderProgram.loadModelMatrix(modelMatrix, "modelMatrix");
  }
};

#endif

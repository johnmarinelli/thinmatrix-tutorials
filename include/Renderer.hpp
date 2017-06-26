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
  int mRenderWidth, mRenderHeight;
  Camera mCamera;
  
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
  
  void render(const Entity& entity, ShaderProgram& shaderProgram) {
    auto model = entity.mModel;
    auto rawModel = model.mRawModel;
    auto texture = model.mModelTexture;
    
    glViewport(0, 0, mRenderWidth, mRenderHeight);
    
    glm::mat4 modelMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
    
    modelMatrix = glm::scale(modelMatrix, entity.mScale);
    modelMatrix = glm::rotate(modelMatrix, entity.mRotationAngle, entity.mRotation);
    modelMatrix = glm::translate(modelMatrix, entity.mPosition);
    
    viewMatrix = mCamera.createViewMatrix();
    
    GLuint modelMatrixLocation, projectionMatrixLocation, viewMatrixLocation;
    projectionMatrixLocation = shaderProgram.uniform("projectionMatrix");
    viewMatrixLocation = shaderProgram.uniform("viewMatrix");
    modelMatrixLocation = shaderProgram.uniform("modelMatrix");
    
    glBindVertexArray(rawModel.mVaoID);
    
    for (auto att : rawModel.mAttributes) {
      glEnableVertexAttribArray(att);
    }
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    shaderProgram.loadShineVariables(texture.mShineDamper, texture.mReflectivity, "shineDamper", "reflectivity");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.mModelTexture.mTextureID);
    glDrawElements(GL_TRIANGLES, rawModel.mIndexCount, GL_UNSIGNED_INT, 0);
    
    for (auto att : rawModel.mAttributes) {
      glDisableVertexAttribArray(att);
    }
    
    glBindVertexArray(0);
  }
};

#endif

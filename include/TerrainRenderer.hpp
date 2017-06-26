#ifndef TerrainRenderer_hpp
#define TerrainRenderer_hpp

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
#include "Terrain.hpp"
#include "TerrainShader.hpp"

class TerrainRenderer {
public:
  TerrainShader mShaderProgram;
  GLFWwindow* mWindowHdl;
  glm::mat4 mProjectionMatrix;
  
  TerrainRenderer() :
    mWindowHdl(nullptr) {
  }
  
  TerrainRenderer(GLFWwindow* window, const TerrainShader& shader) :
    mWindowHdl(window),
    mShaderProgram(shader) {
  }
  
  void init(const glm::mat4& projMatrix) {
    mProjectionMatrix = projMatrix;
    mShaderProgram.use();
    mShaderProgram.loadProjectionMatrix(mProjectionMatrix, "projectionMatrix");
    mShaderProgram.disable();
  }
  
  void render(const std::vector<Terrain>& terrains) {
    for (auto terrain : terrains) {
      prepareTerrain(terrain);
      prepareInstance(terrain);
      glDrawElements(GL_TRIANGLES, terrain.mRawModel.mIndexCount, GL_UNSIGNED_INT, 0);
      unbindTexturedModel(terrain);
    }
  }
  
  void prepareTerrain(const Terrain& model) {
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
  
  void unbindTexturedModel(const Terrain& model) {
    auto rawModel = model.mRawModel;
    for (auto att : rawModel.mAttributes) {
      glDisableVertexAttribArray(att);
    }
  }
  
  void prepareInstance(const Terrain& terrain) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::scale(modelMatrix, glm::vec3{1.0f});
    modelMatrix = glm::translate(modelMatrix, glm::vec3{terrain.mX, 0.0f, terrain.mZ});
    mShaderProgram.loadModelMatrix(modelMatrix, "modelMatrix");
  }
  
};

#endif 

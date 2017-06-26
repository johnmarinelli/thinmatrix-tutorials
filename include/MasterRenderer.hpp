#ifndef MasterRenderer_hpp
#define MasterRenderer_hpp

#include <stdio.h>
#include <map>

#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class MasterRenderer {
public:
  ShaderProgram mShaderProgram;
  Renderer mRenderer;
  GLFWwindow* mWindowHdl;
  
  std::map<TexturedModelType, std::pair<TexturedModel, std::vector<Entity>>> mEntities;
  
  MasterRenderer() :
    mShaderProgram(),
    mWindowHdl(nullptr),
    mRenderer() {    
  }
  
  MasterRenderer(GLFWwindow* window, const ShaderProgram& program) :
    mShaderProgram(program),
    mWindowHdl(window),
    mRenderer(window, program) {    
  }
  
  void init() {
    mRenderer.mWindowHdl = mWindowHdl;
    mRenderer.mShaderProgram = mShaderProgram;
    mRenderer.init();
  }
  
  void addTexturedModel(const TexturedModel& texturedModel) {
    auto texturedModelType = texturedModel.mModelType;
    if (mEntities.find(texturedModelType) == mEntities.end()) {
      mEntities[texturedModelType] = std::pair<TexturedModel, std::vector<Entity>>{texturedModel, std::vector<Entity>{}};
    }
    else {
      auto typeStr = textureModelStrings[texturedModelType];
      throw std::runtime_error("Texture model type: " + typeStr + " already added.");
    }
  }
  
  void addEntity(const Entity& entity, TexturedModelType texturedModelType) {

    if (mEntities.find(texturedModelType) == mEntities.end()) {
      auto typeStr = textureModelStrings[texturedModelType];
      throw std::runtime_error("Texture model type: " + typeStr + " hasn't been added.");
    }
    else {
      mEntities[texturedModelType].second.push_back(entity);      
    }
  }
  
  void render(const Light& sun) {
    mRenderer.prepare();
    
    mShaderProgram.use();
    mShaderProgram.addLight(sun, "lightPosition", "lightColor");
    mShaderProgram.loadAmbientFactor(0.2f, "ambientFactor");

    mShaderProgram.loadProjectionMatrix(mRenderer.mProjectionMatrix, "projectionMatrix");
    mShaderProgram.loadViewMatrix(mRenderer.mCamera.createViewMatrix(), "viewMatrix");
    
    mRenderer.render(mEntities);
    
    mShaderProgram.disable();
  }
  
  void cleanUp() {
  }
};

#endif

#ifndef MasterRenderer_hpp
#define MasterRenderer_hpp

#include <stdio.h>
#include <map>

#include "StaticShader.hpp"
#include "EntityRenderer.hpp"
#include "TerrainShader.hpp"
#include "TerrainRenderer.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class MasterRenderer {
public:
  StaticShader mShaderProgram;
  TerrainShader mTerrainShader;
  EntityRenderer mEntityRenderer;
  TerrainRenderer mTerrainRenderer;
  
  std::vector<Terrain> mTerrains;
  GLFWwindow* mWindowHdl;
  glm::mat4 mProjectionMatrix;
  int mRenderWidth, mRenderHeight;
  
  std::map<TexturedModelType, std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>> mEntities;
  
  MasterRenderer() :
    mShaderProgram(),
    mTerrainShader(),
    mWindowHdl(nullptr),
    mEntityRenderer(),
    mTerrainRenderer() {
  }
  
  MasterRenderer(GLFWwindow* window, const StaticShader& program, const TerrainShader& terrainShader) :
    mShaderProgram(program),
    mTerrainShader(terrainShader),
    mWindowHdl(window),
    mEntityRenderer(window, program),
    mTerrainRenderer(window, terrainShader) {
  }
  
  void init() {
    mEntityRenderer.mWindowHdl = mWindowHdl;
    mEntityRenderer.mShaderProgram = mShaderProgram;
    mTerrainRenderer.mWindowHdl = mWindowHdl;
    mTerrainRenderer.mShaderProgram = mTerrainShader;
    
    glfwGetFramebufferSize(mWindowHdl, &mRenderWidth, &mRenderHeight);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    mProjectionMatrix = createProjectionMatrix(mRenderWidth, mRenderHeight);
    mEntityRenderer.init(mProjectionMatrix);
    mTerrainRenderer.init(mProjectionMatrix);
  }
  
  void addTexturedModel(const TexturedModel& texturedModel) {
    auto texturedModelType = texturedModel.mModelType;
    if (mEntities.find(texturedModelType) == mEntities.end()) {
      mEntities[texturedModelType] = std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>{texturedModel, std::vector<std::shared_ptr<Entity>>{}};
    }
    else {
      auto typeStr = textureModelStrings[texturedModelType];
      throw std::runtime_error("Texture model type: " + typeStr + " already added.");
    }
  }
  
  void addEntity(const std::shared_ptr<Entity>& entity, TexturedModelType texturedModelType) {

    if (mEntities.find(texturedModelType) == mEntities.end()) {
      auto typeStr = textureModelStrings[texturedModelType];
      throw std::runtime_error("Texture model type: " + typeStr + " hasn't been added.");
    }
    else {
      mEntities[texturedModelType].second.push_back(entity);      
    }
  }
  
  void addTerrain(const Terrain& terrain) {
    mTerrains.push_back(terrain);
  }
  
  void render(const Light& sun) {
    prepare();
    
    auto viewMatrix = mEntityRenderer.mCamera.createViewMatrix();
    
    mShaderProgram.use();
    mShaderProgram.loadLight(sun, "lightPosition", "lightColor");
    mShaderProgram.loadAmbientFactor(0.2f, "ambientFactor");
    mShaderProgram.loadViewMatrix(viewMatrix, "viewMatrix");
    mEntityRenderer.render(mEntities);
    mShaderProgram.disable();
    
    mTerrainShader.use();
    mTerrainShader.loadLight(sun, "lightPosition", "lightColor");
    mTerrainShader.loadAmbientFactor(0.2f, "ambientFactor");
    mTerrainShader.loadViewMatrix(viewMatrix, "viewMatrix");
    mTerrainRenderer.render(mTerrains);
    mTerrainShader.disable();
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
    mEntityRenderer.mCamera.move(dir);
  }
  
  void cleanUp() {
  }
};

#endif

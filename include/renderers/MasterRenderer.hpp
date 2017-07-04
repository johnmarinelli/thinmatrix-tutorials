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
  
  glm::vec3 mSkyColor;
  
  std::map<TexturedModelType, std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>> mEntities;
  
  MasterRenderer() :
    mShaderProgram(),
    mTerrainShader(),
    mWindowHdl(nullptr),
    mEntityRenderer(),
    mTerrainRenderer(),
    mSkyColor(glm::vec3{0.5f, 0.5f, 0.5f}) {
  }
  
  MasterRenderer(GLFWwindow* window, const StaticShader& program, const TerrainShader& terrainShader) :
    mShaderProgram(program),
    mTerrainShader(terrainShader),
    mWindowHdl(window),
    mEntityRenderer(window, program),
    mTerrainRenderer(window, terrainShader),
    mSkyColor(glm::vec3{0.5f, 0.5f, 0.5f}) {
  }
  
  void init() {
    mEntityRenderer.mWindowHdl = mWindowHdl;
    mEntityRenderer.mShaderProgram = mShaderProgram;
    mTerrainRenderer.mWindowHdl = mWindowHdl;
    mTerrainRenderer.mShaderProgram = mTerrainShader;
    
    glfwGetFramebufferSize(mWindowHdl, &mRenderWidth, &mRenderHeight);
    mProjectionMatrix = createProjectionMatrix(mRenderWidth, mRenderHeight);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
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
  
  void addEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType) {
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
    mShaderProgram.loadProjectionMatrix(mProjectionMatrix, "projectionMatrix");
    mShaderProgram.loadLight(sun, "lightPosition", "lightColor");
    mShaderProgram.loadAmbientFactor(0.2f, "ambientFactor");
    mShaderProgram.loadViewMatrix(viewMatrix, "viewMatrix");
    mShaderProgram.loadSkyColor(mSkyColor, "skyColor");
    mEntityRenderer.render(mEntities);
    mShaderProgram.disable();
    
    mTerrainShader.use();
    mTerrainShader.loadLight(sun, "lightPosition", "lightColor");
    mTerrainShader.loadAmbientFactor(0.2f, "ambientFactor");
    mTerrainShader.loadViewMatrix(viewMatrix, "viewMatrix");
    mTerrainShader.loadSkyColor(mSkyColor, "skyColor");
    mTerrainRenderer.render(mTerrains);
    mTerrainShader.disable();
    
  }
  
  void prepare() {
    glClearColor(mSkyColor.r, mSkyColor.g, mSkyColor.b, 1.0f);
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
  
  void cleanUp() {
  }
};

#endif

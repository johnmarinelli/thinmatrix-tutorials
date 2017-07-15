#include "MasterRenderer.hpp"


MasterRenderer::MasterRenderer() :
  mStaticShader(),
  mTerrainShader(),
  mNormalShader(),
  mEntityRenderer(),
  mTerrainRenderer(),
  mSkyboxRenderer(),
  mNormalRenderer(),
  mSkyColor(glm::vec3{0.1f, 0.1f, 0.1f}) {
}

void MasterRenderer::init(GLFWwindow* window, Loader& loader) {
  mEntityRenderer.mShaderProgram = mStaticShader;
  mTerrainRenderer.mShaderProgram = mTerrainShader;
  mSkyboxRenderer.mShaderProgram = mSkyboxShader;
  mNormalRenderer.mShaderProgram = mNormalShader;
  
  glfwGetFramebufferSize(window, &mRenderWidth, &mRenderHeight);
  mProjectionMatrix = createProjectionMatrix(mRenderWidth, mRenderHeight);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  mEntityRenderer.init(mProjectionMatrix);
  mTerrainRenderer.init(mProjectionMatrix);
  mSkyboxRenderer.init(loader, mProjectionMatrix);
  mNormalRenderer.init(mProjectionMatrix, mEntityRenderer.mCamera);
  
  mStaticShader.use();
  mStaticShader.loadProjectionMatrix(mProjectionMatrix);
  mStaticShader.loadAmbientFactor(0.2f);
  mStaticShader.loadSkyColor(mSkyColor);
  mStaticShader.disable();
  
  mTerrainShader.use();
  mTerrainShader.loadProjectionMatrix(mProjectionMatrix);
  mTerrainShader.loadAmbientFactor(0.2f);
  mTerrainShader.loadSkyColor(mSkyColor);
  mTerrainShader.disable();
}

void MasterRenderer::addTexturedModel(const TexturedModel& texturedModel) {
  auto texturedModelType = texturedModel.mModelType;
  if (mEntities.find(texturedModelType) == mEntities.end()) {
    mEntities[texturedModelType] = std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>{texturedModel, std::vector<std::shared_ptr<Entity>>{}};
  }
  else {
    auto typeStr = textureModelStrings[texturedModelType];
    throw std::runtime_error("Texture model type: " + typeStr + " already added.");
  }
}

void MasterRenderer::addNormalTexturedModel(const TexturedModel& texturedModel) {
  auto texturedModelType = texturedModel.mModelType;
  if (mNormalEntities.find(texturedModelType) == mNormalEntities.end()) {
    mNormalEntities[texturedModelType] = std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>{texturedModel, std::vector<std::shared_ptr<Entity>>{}};
  }
  else {
    auto typeStr = textureModelStrings[texturedModelType];
    throw std::runtime_error("Texture model type: " + typeStr + " already added.");
  }
}

void MasterRenderer::addEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType) {
  if (mEntities.find(texturedModelType) == mEntities.end()) {
    auto typeStr = textureModelStrings[texturedModelType];
    throw std::runtime_error("Texture model type: " + typeStr + " hasn't been added.");
  }
  else {
    mEntities[texturedModelType].second.push_back(entity);
  }
}

void MasterRenderer::addNormalEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType) {
  if (mNormalEntities.find(texturedModelType) == mNormalEntities.end()) {
    auto typeStr = textureModelStrings[texturedModelType];
    throw std::runtime_error("Texture model type: " + typeStr + " hasn't been added.");
  }
  else {
    mNormalEntities[texturedModelType].second.push_back(entity);
  }
}

void MasterRenderer::addTerrain(const Terrain& terrain) {
  mTerrains.push_back(terrain);
}

void MasterRenderer::update(double dt) {
  mSkyboxRenderer.update(dt);
  mEntityRenderer.mCamera.update(dt);
}

void MasterRenderer::render(const std::vector<Light>& lights) {
  prepare();
  
  auto viewMatrix = mEntityRenderer.mCamera.createViewMatrix();
  
  mStaticShader.use();
  mStaticShader.loadLights(lights);
  mStaticShader.loadViewMatrix(viewMatrix);
  mEntityRenderer.render(mEntities);
  mStaticShader.disable();

  mNormalRenderer.render(mNormalEntities, lights, mSkyColor);
  
  mTerrainShader.use();
  mTerrainShader.loadLights(lights);
  mTerrainShader.loadViewMatrix(viewMatrix);
  mTerrainRenderer.render(mTerrains);
  mTerrainShader.disable();
  
  mSkyboxRenderer.render(mEntityRenderer.mCamera, mSkyColor);

}

void MasterRenderer::prepare() {
  glClearColor(mSkyColor.r, mSkyColor.g, mSkyColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::mat4 MasterRenderer::createProjectionMatrix(int width, int height) const {
  float ratio;
  ratio = width / (float) height;
  
  glm::mat4 projection;
  
  //projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
  projection = glm::perspective(70.0f, ratio, 0.1f, 1000.0f);
  glViewport(0, 0, width, height);
  
  return projection;
}

void MasterRenderer::cleanUp() {
  mEntityRenderer.cleanUp();
  mTerrainRenderer.cleanUp();
  mNormalRenderer.cleanUp();
}

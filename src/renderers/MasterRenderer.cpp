#include "MasterRenderer.hpp"


MasterRenderer::MasterRenderer() :
  mStaticShader(),
  mTerrainShader(),
  mWindowHdl(nullptr),
  mEntityRenderer(),
  mTerrainRenderer(),
  mSkyColor(glm::vec3{0.5f, 0.5f, 0.5f}) {
}

MasterRenderer::MasterRenderer(GLFWwindow* window, const StaticShader& program, const TerrainShader& terrainShader) :
  mStaticShader(program),
  mTerrainShader(terrainShader),
  mWindowHdl(window),
  mEntityRenderer(window, program),
  mTerrainRenderer(window, terrainShader),
  mSkyColor(glm::vec3{0.5f, 0.5f, 0.5f}) {
}

void MasterRenderer::init() {
  mEntityRenderer.mWindowHdl = mWindowHdl;
  mEntityRenderer.mShaderProgram = mStaticShader;
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

void MasterRenderer::addEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType) {
  if (mEntities.find(texturedModelType) == mEntities.end()) {
    auto typeStr = textureModelStrings[texturedModelType];
    throw std::runtime_error("Texture model type: " + typeStr + " hasn't been added.");
  }
  else {
    mEntities[texturedModelType].second.push_back(entity);
  }
}

void MasterRenderer::addTerrain(const Terrain& terrain) {
  mTerrains.push_back(terrain);
}

void MasterRenderer::render(const Light& sun) {
  prepare();
  
  auto viewMatrix = mEntityRenderer.mCamera.createViewMatrix();
  
  mStaticShader.use();
  mStaticShader.loadProjectionMatrix(mProjectionMatrix, "projectionMatrix");
  mStaticShader.loadLight(sun, "lightPosition", "lightColor");
  mStaticShader.loadAmbientFactor(0.2f, "ambientFactor");
  mStaticShader.loadViewMatrix(viewMatrix, "viewMatrix");
  mStaticShader.loadSkyColor(mSkyColor, "skyColor");
  mEntityRenderer.render(mEntities);
  mStaticShader.disable();
  
  mTerrainShader.use();
  mTerrainShader.loadLight(sun, "lightPosition", "lightColor");
  mTerrainShader.loadAmbientFactor(0.2f, "ambientFactor");
  mTerrainShader.loadViewMatrix(viewMatrix, "viewMatrix");
  mTerrainShader.loadSkyColor(mSkyColor, "skyColor");
  mTerrainRenderer.render(mTerrains);
  mTerrainShader.disable();
  
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
}

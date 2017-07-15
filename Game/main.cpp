#include <iostream>
#include <iterator>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>

#include "RawModel.hpp"
#include "MasterRenderer.hpp"
#include "Loader.hpp"
#include "ShaderProgram.hpp"
#include "ModelTexture.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "ObjLoader.hpp"
#include "NormalObjLoader.hpp"
#include "Light.hpp"
#include "Player.hpp"
#include "Terrain.hpp"
#include "TerrainTexturePack.hpp"
#include "Timer.hpp"
#include "GUIRenderer.hpp"
#include "MousePicker.hpp"

std::shared_ptr<Player> player;
MasterRenderer masterRenderer;
MousePicker mousePicker;
double lastMouseXPosition = -1.0;
double lastMouseYPosition = -1.0;
bool leftMouseButtonDown = false;
bool rightMouseButtonDown = false;

static void errorCallback(int err, const char* desc) {
  fprintf(stderr, "Error: %s\n", desc);
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  masterRenderer.mEntityRenderer.mCamera.zoom(yOffset);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (GLFW_MOUSE_BUTTON_RIGHT == button) {
    if (GLFW_PRESS == action) rightMouseButtonDown = true;
    if (GLFW_RELEASE == action) rightMouseButtonDown = false;
  }
  if (GLFW_MOUSE_BUTTON_LEFT == button) {
    if (GLFW_PRESS == action) {
      leftMouseButtonDown = true;
      double xPos, yPos;
      glfwGetCursorPos(window, &xPos, &yPos);
      mousePicker.calculateMouseRay((float) xPos, (float) yPos);
    }
    if (GLFW_RELEASE == action) leftMouseButtonDown = false;
  }
  
  if (GLFW_RELEASE == action) {
    lastMouseXPosition = -1;
    lastMouseYPosition = -1;
  }
}

static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
  if (leftMouseButtonDown || rightMouseButtonDown) {    
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (lastMouseYPosition == -1 && lastMouseXPosition == -1) {
      lastMouseXPosition = xPos;
      lastMouseYPosition = yPos;
    }
    
    double xd = lastMouseXPosition - xPos;
    double yd = lastMouseYPosition - yPos;
    
    if (leftMouseButtonDown) masterRenderer.mEntityRenderer.mCamera.rotate(xd);
    if (rightMouseButtonDown) masterRenderer.mEntityRenderer.mCamera.moveVertical(yd);
    
    lastMouseXPosition = xPos;
    lastMouseYPosition = yPos;
  }
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
  
  switch (key) {
    case GLFW_KEY_W:
      if (action == GLFW_PRESS) player->handleInput(PlayerMovementDirection::UP, GLFW_PRESS);
      if (action == GLFW_RELEASE) player->handleInput(PlayerMovementDirection::UP, GLFW_RELEASE);
      break;
    case GLFW_KEY_S:
      if (action == GLFW_PRESS) player->handleInput(PlayerMovementDirection::DOWN, GLFW_PRESS);
      if (action == GLFW_RELEASE) player->handleInput(PlayerMovementDirection::DOWN, GLFW_RELEASE);
      break;
    case GLFW_KEY_A:
      if (action == GLFW_PRESS) player->handleInput(PlayerMovementDirection::LEFT, GLFW_PRESS);
      if (action == GLFW_RELEASE) player->handleInput(PlayerMovementDirection::LEFT, GLFW_RELEASE);
      break;
    case GLFW_KEY_D:
      if (action == GLFW_PRESS) player->handleInput(PlayerMovementDirection::RIGHT, GLFW_PRESS);
      if (action == GLFW_RELEASE) player->handleInput(PlayerMovementDirection::RIGHT, GLFW_RELEASE);
      break;
    case GLFW_KEY_SPACE:
      if (action == GLFW_PRESS) player->handleInput(PlayerMovementDirection::JUMP, GLFW_PRESS);
      break;
    default: break;
  }
}

StaticShader loadStaticShader() {
  StaticShader shaderProgram;
  shaderProgram.init("resources/shaders/game.vert", "resources/shaders/game.frag");
  return shaderProgram;
}

TerrainShader loadTerrainShader() {
  TerrainShader shaderProgram;
  shaderProgram.init("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");
  return shaderProgram;
}

SkyboxShader loadSkyboxShader() {
  SkyboxShader shaderProgram;
  shaderProgram.init("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
  return shaderProgram;
}

NormalShader loadNormalShader() {
  NormalShader shaderProgram;
  shaderProgram.init("resources/shaders/normal.vert", "resources/shaders/normal.frag");
  return shaderProgram;
}

static GLFWwindow* initGLFW() {
  GLFWwindow* window;
  
  if (!glfwInit()) exit(EXIT_FAILURE);
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "Game", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, cursorPositionCallback);
  glfwSetErrorCallback(errorCallback);
  
  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(1);
  
  return window;
}

RawModel loadModel(const std::string& filepath, ObjLoader& objLoader, Loader& loader) {
  ModelData data = objLoader.loadObj(filepath);
  RawModel model = loader.loadToVAO((GLfloat*) &data.mVertices[0],
                                    (GLfloat*) &data.mNormals[0],
                                    (GLfloat*) &data.mTextureCoords[0],
                                    (GLuint*) &data.mIndices[0],
                                    (int) data.mVertices.size(),
                                    (int) data.mNormals.size(),
                                    (int) data.mTextureCoords.size(),
                                    (int) data.mIndices.size());
  return model;
}

RawModel loadNormalModel(const std::string& filepath, NormalObjLoader& objLoader, Loader& loader) {
  ModelDataNM data = objLoader.loadObj(filepath);

  RawModel model = loader.loadToVAO((GLfloat*) &data.mVertices[0],
                                    (GLfloat*) &data.mNormals[0],
                                    (GLfloat*) &data.mTextureCoords[0],
                                    (GLfloat*) &data.mTangents[0],
                                    (GLuint*) &data.mIndices[0],
                                    (int) data.mVertices.size(),
                                    (int) data.mNormals.size(),
                                    (int) data.mTextureCoords.size(),
                                    (int) data.mTangents.size(),
                                    (int) data.mIndices.size());
  return model;
}

struct ModelOptions {
  float mShineDamper;
  float mReflectivity;
  int mNumberOfRows;
  bool mHasTransparency;
  bool mUseFakeLighting;
  
  ModelOptions(float shineDamper, float reflectivity, bool hasTransparency, bool useFakeLighting, int numRows = 1) :
    mShineDamper(shineDamper),
    mReflectivity(reflectivity),
    mNumberOfRows(numRows),
    mHasTransparency(hasTransparency),
    mUseFakeLighting(useFakeLighting) {
  }
};

ModelTexture loadModelTexture(const std::string& filepath, Loader& loader, const ModelOptions& opts) {
  ModelTexture modelTexture{loader.loadTexture(filepath)};
  modelTexture.mShineDamper = opts.mShineDamper;
  modelTexture.mReflectivity = opts.mReflectivity;
  modelTexture.mHasTransparency = opts.mHasTransparency;
  modelTexture.mUseFakeLighting = opts.mUseFakeLighting;
  modelTexture.mNumberOfRows = opts.mNumberOfRows;
  
  return modelTexture;
}

std::shared_ptr<Entity> createEntity(const TexturedModel& texModel, const glm::vec3& pos, const glm::vec3& rot = glm::vec3{0.0f, 1.0f, 0.0f}, float rotAngle = 0.0f, const glm::vec3& scale = glm::vec3{1.0f}) {
  std::shared_ptr<Entity> entity = std::make_shared<Entity>(texModel, 0, pos, rot, scale);
  entity->mRotationAngle = rotAngle;
  return entity;
}

void registerEntity(const std::shared_ptr<Entity> entity, MasterRenderer& masterRenderer, TexturedModelType texturedModelType) {
  // quick & dirty way to ensure entity is rendered at terrain height
  // TODO: change this later  
  entity->mPosition.y = masterRenderer.mTerrains[0].getHeightAtCoord(entity->mPosition.x, entity->mPosition.z);
  
  masterRenderer.addEntity(entity, texturedModelType);
}

void registerNormalEntity(const std::shared_ptr<Entity> entity, MasterRenderer& masterRenderer, TexturedModelType texturedModelType) {
  // quick & dirty way to ensure entity is rendered at terrain height
  // TODO: change this later
  //entity->mPosition.y = masterRenderer.mTerrains[0].getHeightAtCoord(entity->mPosition.x, entity->mPosition.z);
  
  masterRenderer.addNormalEntity(entity, texturedModelType);
}

void generateFerns(MasterRenderer& masterRenderer, const TexturedModel& fernTexModel) {
  int numFerns = 400;
  
  for (auto i = 0; i < numFerns; ++i) {
    auto rx = random(0.0f, 800.0f);
    auto rz = random(0.0f, 800.0f);
    auto pos = glm::vec3{rx, 0.0f, rz};
    
    auto fern = createEntity(fernTexModel, pos);
    registerEntity(fern, masterRenderer, TexturedModelType::FERN);
  }
}

int main(int argc, const char * argv[]) {
  srand(static_cast<unsigned>(time(0)));
  Timer timer;
  GLFWwindow* window = initGLFW();
  
  Loader loader;
  
  /*
   * Terrain textures
   */
  GLuint bgTexture = loader.loadTexture("resources/textures/grassy2.png");
  GLuint rTexture = loader.loadTexture("resources/textures/mud.png");
  GLuint gTexture = loader.loadTexture("resources/textures/grassFlowers.png");
  GLuint bTexture = loader.loadTexture("resources/textures/path.png");
  GLuint blendMap = loader.loadTexture("resources/textures/blendMap.png");
  
  /*
   * Texture pack for terrain
   */
  TerrainTexturePack texturePack{bgTexture, rTexture, gTexture, bTexture};
  Terrain terrain{0, 0, loader, texturePack, blendMap, "resources/textures/heightmap.png"};
  masterRenderer.addTerrain(terrain);
  
  /*
   * Lights
   */
  StaticShader staticShader = loadStaticShader();
  TerrainShader terrainShader = loadTerrainShader();
  SkyboxShader skyboxShader = loadSkyboxShader();
  NormalShader normalShader = loadNormalShader();
  glm::vec3 lightPos0{0.0f, 10000.0f, -7000.0f};
  glm::vec3 lightCol0{0.7f, 0.7f, 0.7f};
  Light light0{lightPos0, lightCol0};
  
  glm::vec3 lightPos1{185.0f, -4.7f, 23.0f};
  glm::vec3 lightCol1{3.0f, 0.0f, 0.0f};
  glm::vec3 attenuation1{1.0f, 0.01f, 0.002f};
  Light light1{lightPos1, lightCol1, attenuation1};
  
  glm::vec3 lightPos2{370.0f, 17.0f, 300.0f};
  glm::vec3 lightCol2{0.0f, 2.0f, 2.0f};
  glm::vec3 attenuation2{1.0f, 0.01f, 0.002f};
  Light light2{lightPos2, lightCol2, attenuation2};
  
  glm::vec3 lightPos3{293.0f, 7.0f, 305.0f};
  glm::vec3 lightCol3{2.0f, 2.0f, 0.0f};
  glm::vec3 attenuation3{1.0f, 0.01f, 0.002f};
  Light light3{lightPos3, lightCol3, attenuation3};
  
  std::vector<Light> lights{light0, light1, light2, light3};

  /*
   * Models
   */
  ObjLoader objLoader;
  NormalObjLoader normalObjLoader;
  
  masterRenderer.mStaticShader = staticShader;
  masterRenderer.mTerrainShader = terrainShader;
  masterRenderer.mSkyboxShader = skyboxShader;
  masterRenderer.mNormalShader = normalShader;
  masterRenderer.init(window, loader);
  
  RawModel dragon = loadModel("resources/meshes/dragon.obj", objLoader, loader);
  RawModel fern = loadModel("resources/meshes/fern.obj", objLoader, loader);
  RawModel grass = loadModel("resources/meshes/grassModel.obj", objLoader, loader);
  RawModel playerModel = loadModel("resources/meshes/player.obj", objLoader, loader);
  RawModel lamp = loadModel("resources/meshes/lamp.obj", objLoader, loader);
  RawModel barrel = loadNormalModel("resources/meshes/barrel.obj", normalObjLoader, loader);
  
  ModelTexture modelTexture = loadModelTexture("resources/textures/scales.png", loader, {1.0f, 1.0f, false, false});
  ModelTexture fernTexture = loadModelTexture("resources/textures/fern.png", loader, {1.0f, 0.0f, true, false, 2});
  ModelTexture grassTexture = loadModelTexture("resources/textures/grassTexture.png", loader, {1.0f, 1.0f, true, true});
  ModelTexture playerTexture = loadModelTexture("resources/textures/playerTexture.png", loader, {1.0f, 1.0f, false, false});
  ModelTexture lampTexture = loadModelTexture("resources/textures/lamp.png", loader, {1.0f, 1.0f, false, false});
  ModelTexture barrelTexture = loadModelTexture("resources/textures/barrel.png", loader, {10.0f, 0.5f, false, false});
  
  TexturedModel texturedModel{dragon, modelTexture, TexturedModelType::DRAGON};
  TexturedModel fernTexturedModel{fern, fernTexture, TexturedModelType::FERN};
  TexturedModel grassTexturedModel{grass, grassTexture, TexturedModelType::GRASS};
  TexturedModel playerTexturedModel{playerModel, playerTexture, TexturedModelType::PLAYER};
  TexturedModel lampTexturedModel{lamp, lampTexture, TexturedModelType::LAMP};
  TexturedModel barrelTexturedModel{barrel, barrelTexture, TexturedModelType::BARREL};
  barrelTexturedModel.mModelTexture.mNormalMapID = loader.loadTexture("resources/textures/barrelNormal.png");

  auto entity = createEntity(texturedModel, glm::vec3{50.0f, 0.0f, 30.0f});
  auto fernEntity = createEntity(fernTexturedModel, glm::vec3{75.0f, 0.0f, 25.0f});
  auto grassEntity = createEntity(grassTexturedModel, glm::vec3{25.0f, 0.0f, 25.0f});
  auto lampEntity = createEntity(lampTexturedModel, glm::vec3{185.0f, -4.7f, 23.0f});
  auto lampEntity2 = createEntity(lampTexturedModel, glm::vec3{370.0f, 17.0f, 300.0f});
  auto lampEntity3 = createEntity(lampTexturedModel, glm::vec3{293.0f, 7.0f, 305.0f});
  auto barrelEntity = createEntity(barrelTexturedModel, glm::vec3{210.0f, 10.0f, 300.0f});
  
  masterRenderer.addTexturedModel(texturedModel);
  masterRenderer.addTexturedModel(fernTexturedModel);
  masterRenderer.addTexturedModel(grassTexturedModel);
  masterRenderer.addTexturedModel(playerTexturedModel);
  masterRenderer.addTexturedModel(lampTexturedModel);
  masterRenderer.addNormalTexturedModel(barrelTexturedModel);
  
  registerEntity(entity, masterRenderer, TexturedModelType::DRAGON);
  registerEntity(fernEntity, masterRenderer, TexturedModelType::FERN);
  registerEntity(grassEntity, masterRenderer, TexturedModelType::GRASS);
  registerEntity(lampEntity, masterRenderer, TexturedModelType::LAMP);
  registerEntity(lampEntity2, masterRenderer, TexturedModelType::LAMP);
  registerEntity(lampEntity3, masterRenderer, TexturedModelType::LAMP);
  registerNormalEntity(barrelEntity, masterRenderer, TexturedModelType::BARREL);
  
  generateFerns(masterRenderer, fernTexturedModel);

  player = std::make_shared<Player>(playerTexturedModel, glm::vec3{200.0f, 10.0f, 290.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f});
  masterRenderer.mEntityRenderer.mCamera.mPlayerHdl = player;
  registerEntity(player, masterRenderer, TexturedModelType::PLAYER);
  
  std::vector<GUITexture> guis;
  GUITexture gui{loader.loadTexture("resources/textures/puppy.png"), glm::vec2{0.9f, 0.9f}, glm::vec2{0.2f, 0.2f}};
  guis.push_back(gui);
  
  GUIRenderer guiRenderer{loader};
  mousePicker.init(masterRenderer.mProjectionMatrix, masterRenderer.mEntityRenderer.mCamera, masterRenderer.mTerrains[0]);
  
  while (!glfwWindowShouldClose(window)) {
    timer.update();
    delta = timer.getDelta();
    
    glfwPollEvents();

    while (delta > 0.0) {
      dt = std::min(delta, timer.DT);
      player->update(dt, masterRenderer.mTerrains[0]);
      masterRenderer.update(dt);
      mousePicker.update(masterRenderer.mEntityRenderer.mCamera.createViewMatrix());
      delta -= dt;      
    }
    masterRenderer.render(lights);
    guiRenderer.render(guis);
    
    glfwSwapBuffers(window);
    
    int glErr = glGetError();
    
    if (0 != glErr) {
      std::cout << "OPENGL ERROR! code: " << glErr << '\n';
      const GLubyte *errString = gluErrorString(glErr);
      std::cout << errString << '\n';
    }
  }
  
  masterRenderer.cleanUp();
  loader.cleanUp();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

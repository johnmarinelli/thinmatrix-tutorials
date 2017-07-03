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
#include "Light.hpp"
#include "Player.hpp"
#include "Terrain.hpp"
#include "TerrainTexturePack.hpp"
#include "Timer.hpp"

const int WIDTH = 640;
const int HEIGHT = 480;

std::shared_ptr<Player> player;
MasterRenderer masterRenderer;
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
    if (GLFW_PRESS == action) leftMouseButtonDown = true;
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

StaticShader loadShaders() {
  StaticShader shaderProgram;
  shaderProgram.initFromFiles("resources/shaders/game.vert", "resources/shaders/game.frag");
  
  shaderProgram.registerUniform("projectionMatrix");
  shaderProgram.registerUniform("viewMatrix");
  shaderProgram.registerUniform("modelMatrix");
  shaderProgram.registerUniform("lightPosition");
  shaderProgram.registerUniform("lightColor");
  shaderProgram.registerUniform("shineDamper");
  shaderProgram.registerUniform("reflectivity");
  shaderProgram.registerUniform("ambientFactor");
  shaderProgram.registerUniform("useFakeLighting");
  shaderProgram.registerUniform("skyColor");
  shaderProgram.registerUniform("numTextureRows");
  shaderProgram.registerUniform("textureAtlasXYOffset");
  
  return shaderProgram;
}

TerrainShader loadTerrainShader() {
  TerrainShader shaderProgram;
  shaderProgram.initFromFiles("resources/shaders/terrain.vert", "resources/shaders/terrain.frag");
  
  shaderProgram.registerUniform("projectionMatrix");
  shaderProgram.registerUniform("viewMatrix");
  shaderProgram.registerUniform("modelMatrix");
  shaderProgram.registerUniform("lightPosition");
  shaderProgram.registerUniform("lightColor");
  shaderProgram.registerUniform("shineDamper");
  shaderProgram.registerUniform("reflectivity");
  shaderProgram.registerUniform("ambientFactor");
  shaderProgram.registerUniform("skyColor");
  shaderProgram.registerUniform("backgroundTexture");
  shaderProgram.registerUniform("rTexture");
  shaderProgram.registerUniform("gTexture");
  shaderProgram.registerUniform("bTexture");
  shaderProgram.registerUniform("blendMap");
  
  return shaderProgram;
}

static GLFWwindow* initGLFW() {
  GLFWwindow* window;
  glfwSetErrorCallback(errorCallback);
  
  if (!glfwInit()) exit(EXIT_FAILURE);
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(WIDTH, HEIGHT, "Game", NULL, NULL);
  
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

void generateFerns(MasterRenderer& masterRenderer, const TexturedModel& fernTexModel) {
  int numFerns = 800;
  
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
  
  TerrainTexturePack texturePack{bgTexture, rTexture, gTexture, bTexture};
  Terrain terrain{0, 0, loader, texturePack, blendMap, "resources/textures/heightmap.png"};
  masterRenderer.addTerrain(terrain);
  
  StaticShader shaderProgram = loadShaders();
  TerrainShader terrainShader = loadTerrainShader();
  glm::vec3 lightPos{10.0f, 5000.0f, -400.0f};
  glm::vec3 lightCol{1.0f, 1.0f, 1.0f};
  Light light{lightPos, lightCol};
  
  ObjLoader objLoader;
  masterRenderer.mWindowHdl = window;
  masterRenderer.mShaderProgram = shaderProgram;
  masterRenderer.mTerrainShader = terrainShader;
  masterRenderer.init();
  
  RawModel dragon = loadModel("resources/meshes/dragon.obj", objLoader, loader);
  RawModel fern = loadModel("resources/meshes/fern.obj", objLoader, loader);
  RawModel grass = loadModel("resources/meshes/grassModel.obj", objLoader, loader);
  RawModel playerModel = loadModel("resources/meshes/player.obj", objLoader, loader);

  ModelTexture modelTexture = loadModelTexture("resources/textures/scales.png", loader, {10.0f, 1.0f, false, false});
  ModelTexture fernTexture = loadModelTexture("resources/textures/fern.png", loader, {10.0f, 1.0f, true, false, 2});
  ModelTexture grassTexture = loadModelTexture("resources/textures/grassTexture.png", loader, {10.0f, 1.0f, true, true});
  ModelTexture playerTexture = loadModelTexture("resources/textures/playerTexture.png", loader, {10.0f, 1.0f, false, false});
  
  TexturedModel texturedModel{dragon, modelTexture, TexturedModelType::DRAGON};
  TexturedModel fernTexturedModel{fern, fernTexture, TexturedModelType::FERN};
  TexturedModel grassTexturedModel{grass, grassTexture, TexturedModelType::GRASS};
  TexturedModel playerTexturedModel{playerModel, playerTexture, TexturedModelType::PLAYER};

  auto entity = createEntity(texturedModel, glm::vec3{50.0f, 0.0f, 30.0f});
  auto fernEntity = createEntity(fernTexturedModel, glm::vec3{75.0f, 0.0f, 25.0f});
  auto grassEntity = createEntity(grassTexturedModel, glm::vec3{25.0f, 0.0f, 25.0f});
  masterRenderer.addTexturedModel(texturedModel);
  masterRenderer.addTexturedModel(fernTexturedModel);
  masterRenderer.addTexturedModel(grassTexturedModel);
  masterRenderer.addTexturedModel(playerTexturedModel);
  
  registerEntity(entity, masterRenderer, TexturedModelType::DRAGON);
  registerEntity(fernEntity, masterRenderer, TexturedModelType::FERN);
  registerEntity(grassEntity, masterRenderer, TexturedModelType::GRASS);
  
  generateFerns(masterRenderer, fernTexturedModel);

  player = std::make_shared<Player>(playerTexturedModel, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f});
  masterRenderer.mEntityRenderer.mCamera.mPlayerHdl = player;
  registerEntity(player, masterRenderer, TexturedModelType::PLAYER);
  
  while (!glfwWindowShouldClose(window)) {
    timer.update();
    delta = timer.getDelta();
    
    glfwPollEvents();

    while (delta > 0.0) {
      dt = std::min(delta, timer.DT);
      player->update(dt, masterRenderer.mTerrains[0]);
      masterRenderer.mEntityRenderer.mCamera.update(dt);
      delta -= dt;      
    }
    masterRenderer.render(light);
    glfwSwapBuffers(window);
  }
  
  loader.cleanUp();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

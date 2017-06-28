#include <iostream>
#include <iterator>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
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
#include "Terrain.hpp"

const int WIDTH = 640;
const int HEIGHT = 480;

MasterRenderer masterRenderer;

static void errorCallback(int err, const char* desc) {
  fprintf(stderr, "Error: %s\n", desc);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
  
  if (GLFW_PRESS == action || GLFW_REPEAT == action) {
    if (GLFW_KEY_W == key) {
      masterRenderer.moveCamera(MovementDirection::UP);
    }
    else if(GLFW_KEY_D == key) {
      masterRenderer.moveCamera(MovementDirection::RIGHT);
    }
    else if (GLFW_KEY_A == key) {
      masterRenderer.moveCamera(MovementDirection::LEFT);
    }
    else if (GLFW_KEY_S == key) {
      masterRenderer.moveCamera(MovementDirection::DOWN);
    }
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
  
  glfwSetKeyCallback(window, keyCallback);
  
  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(1);
  
  return window;
}

int main(int argc, const char * argv[]) {
  GLFWwindow* window = initGLFW();
  
  StaticShader shaderProgram = loadShaders();
  TerrainShader terrainShader = loadTerrainShader();
  glm::vec3 lightPos{10.0f, 50.0f, -400.0f};
  glm::vec3 lightCol{1.0f, 1.0f, 1.0f};
  Light light{lightPos, lightCol};
  
  Loader loader;
  
  ObjLoader objLoader;
  masterRenderer.mWindowHdl = window;
  masterRenderer.mShaderProgram = shaderProgram;
  masterRenderer.mTerrainShader = terrainShader;
  masterRenderer.init();
  
  ModelData dragonData = objLoader.loadObj("resources/meshes/dragon.obj");
  RawModel dragon = loader.loadToVAO((GLfloat*) &dragonData.mVertices[0], (GLuint*) &dragonData.mIndices[0], (GLfloat*) &dragonData.mTextureCoords[0], (GLfloat*) &dragonData.mNormals[0], dragonData.mVertices.size(), dragonData.mIndices.size(), dragonData.mTextureCoords.size(), dragonData.mNormals.size());
  
  ModelData fernData = objLoader.loadObj("resources/meshes/fern.obj");
  RawModel fern = loader.loadToVAO((GLfloat*) &fernData.mVertices[0], (GLuint*) &fernData.mIndices[0], (GLfloat*) &fernData.mTextureCoords[0], (GLfloat*) &fernData.mNormals[0], fernData.mVertices.size(), fernData.mIndices.size(), fernData.mTextureCoords.size(), fernData.mNormals.size());
  
  ModelData grassData = objLoader.loadObj("resources/meshes/grassModel.obj");
  RawModel grass = loader.loadToVAO((GLfloat*) &grassData.mVertices[0], (GLuint*) &grassData.mIndices[0], (GLfloat*) &grassData.mTextureCoords[0], (GLfloat*) &grassData.mNormals[0], grassData.mVertices.size(), grassData.mIndices.size(), grassData.mTextureCoords.size(), grassData.mNormals.size());

  ModelTexture modelTexture{loader.loadTexture("resources/textures/scales.png")};
  ModelTexture fernTexture{loader.loadTexture("resources/textures/fern.png")};
  ModelTexture grassTexture{loader.loadTexture("resources/textures/grassTexture.png")};

  modelTexture.mShineDamper = 0.5f;
  modelTexture.mReflectivity = 1.0f;
  
  fernTexture.mShineDamper = 10.0f;
  fernTexture.mReflectivity = 1.0f;
  fernTexture.mHasTransparency = true;
  
  grassTexture.mShineDamper = 10.0f;
  grassTexture.mReflectivity = 1.0f;
  grassTexture.mHasTransparency = true;
  grassTexture.mUseFakeLighting = true;
  
  TexturedModel texturedModel{dragon, modelTexture, TexturedModelType::DRAGON};
  TexturedModel fernTexturedModel{fern, fernTexture, TexturedModelType::FERN};
  TexturedModel grassTexturedModel{grass, grassTexture, TexturedModelType::GRASS};
  
  std::shared_ptr<Entity> entity = std::make_shared<Entity>(texturedModel, glm::vec3{50.0f, 0.0f, -30.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f});
  std::shared_ptr<Entity> fernEntity = std::make_shared<Entity>(fernTexturedModel, glm::vec3{75.0f, 0.0f, -25.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f});
  std::shared_ptr<Entity> grassEntity = std::make_shared<Entity>(grassTexturedModel, glm::vec3{25.0f, 0.0f, -25.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f});
  
  ModelTexture terrainTexture{loader.loadTexture("resources/textures/grassy2.png")};
  terrainTexture.mShineDamper = 0.0f;
  terrainTexture.mReflectivity = 1.0f;
  
  Terrain terrain{0, -1, loader, terrainTexture};
  Terrain terrain2{0, 0, loader, terrainTexture};
  
  masterRenderer.addTexturedModel(texturedModel);
  masterRenderer.addTexturedModel(fernTexturedModel);
  masterRenderer.addTexturedModel(grassTexturedModel);
  masterRenderer.addEntity(entity, TexturedModelType::DRAGON);
  masterRenderer.addEntity(fernEntity, TexturedModelType::FERN);
  masterRenderer.addEntity(grassEntity, TexturedModelType::GRASS);
  masterRenderer.addTerrain(terrain);
  masterRenderer.addTerrain(terrain2);
  
  while (!glfwWindowShouldClose(window)) {
    masterRenderer.render(light);
    entity->mRotationAngle = glfwGetTime();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  loader.cleanUp();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

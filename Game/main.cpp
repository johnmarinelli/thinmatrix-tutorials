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

ShaderProgram loadShaders() {
  ShaderProgram shaderProgram;
  shaderProgram.initFromFiles("resources/shaders/game.vert", "resources/shaders/game.frag");
  
  shaderProgram.registerUniform("projectionMatrix");
  shaderProgram.registerUniform("viewMatrix");
  shaderProgram.registerUniform("modelMatrix");
  shaderProgram.registerUniform("lightPosition");
  shaderProgram.registerUniform("lightColor");
  shaderProgram.registerUniform("shineDamper");
  shaderProgram.registerUniform("reflectivity");
  shaderProgram.registerUniform("ambientFactor");
  
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
  
  ShaderProgram shaderProgram = loadShaders();
  glm::vec3 lightPos{0.0f, 0.0f, -20.0f};
  glm::vec3 lightCol{1.0f, 1.0f, 1.0f};
  Light light{lightPos, lightCol};
  
  Loader loader;
  
  ObjLoader objLoader;
  masterRenderer.mWindowHdl = window;
  masterRenderer.mShaderProgram = shaderProgram;
  masterRenderer.init();
  
  RawModel model = objLoader.loadObjModel("resources/meshes/dragon.obj", loader);
  ModelTexture modelTexture{loader.loadTexture("resources/textures/scales.png")};
  modelTexture.mShineDamper = 1.0f;
  modelTexture.mReflectivity = 1.0f;
  TexturedModel texturedModel{model, modelTexture, TexturedModelType::DRAGON};
  std::shared_ptr<Entity> entity = std::make_shared<Entity>(texturedModel, glm::vec3{0.0f, 0.0f, -15.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f});
  
  masterRenderer.addTexturedModel(texturedModel);
  masterRenderer.addEntity(entity, TexturedModelType::DRAGON);
  
  while (!glfwWindowShouldClose(window)) {
    masterRenderer.render(light);
    entity->mRotationAngle = glfwGetTime() * 0.25;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  loader.cleanUp();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

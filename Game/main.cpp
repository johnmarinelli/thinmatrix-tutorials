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
#include "Renderer.hpp"
#include "Loader.hpp"
#include "ShaderProgram.hpp"
#include "ModelTexture.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "ObjLoader.hpp"
#include "Light.hpp"

const int WIDTH = 640;
const int HEIGHT = 480;

Renderer renderer;

static void errorCallback(int err, const char* desc) {
  fprintf(stderr, "Error: %s\n", desc);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
  
  if (GLFW_PRESS == action || GLFW_REPEAT == action) {
    if (GLFW_KEY_W == key) {
      renderer.moveCamera(MovementDirection::UP);
    }
    else if(GLFW_KEY_D == key) {
      renderer.moveCamera(MovementDirection::RIGHT);
      
    }
    else if (GLFW_KEY_A == key) {
      renderer.moveCamera(MovementDirection::LEFT);
    }
    else if (GLFW_KEY_S == key) {
      renderer.moveCamera(MovementDirection::DOWN);      
    }
  }
}

ShaderProgram loadShaders() {
  ShaderProgram shaderProgram;
  shaderProgram.initFromFiles("resources/shaders/game.vert", "resources/shaders/game.frag");
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
  
  shaderProgram.addUniform("projectionMatrix");
  shaderProgram.addUniform("viewMatrix");
  shaderProgram.addUniform("modelMatrix");
  shaderProgram.addUniform("lightPosition");
  shaderProgram.addUniform("lightColor");
  shaderProgram.addUniform("shineDamper");
  shaderProgram.addUniform("reflectivity");
  shaderProgram.addUniform("ambientFactor");
  
  glm::vec3 lightPos{0.0f, 0.0f, -20.0f};
  glm::vec3 lightCol{1.0f, 1.0f, 1.0f};
  Light light{lightPos, lightCol};
  
  Loader loader;
  
  ObjLoader objLoader;
  
  RawModel model = objLoader.loadObjModel("resources/meshes/dragon.obj", loader);

  renderer.mWindowHdl = window;
  renderer.init();
  
  ModelTexture modelTexture{loader.loadTexture("resources/textures/scales.png")};
  modelTexture.mShineDamper = 1.0f;
  modelTexture.mReflectivity = 1.0f;
  TexturedModel texturedModel{model, modelTexture};
  Entity entity{texturedModel, glm::vec3{0.0f, 0.0f, -25.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{1.0f}};
  
  while (!glfwWindowShouldClose(window)) {
    renderer.prepare();
    
    shaderProgram.use();
    shaderProgram.addLight(light, "lightPosition", "lightColor");
    shaderProgram.loadAmbientFactor(0.2f, "ambientFactor");
    
    entity.mRotationAngle = glfwGetTime();
    renderer.render(entity, shaderProgram);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  loader.cleanUp();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

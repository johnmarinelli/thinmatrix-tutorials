#ifndef Renderer_hpp
#define Renderer_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Entity.hpp"
#include "StaticShader.hpp"
#include "Utilities.h"
#include "Camera.hpp"

class GLFWwindow;

class EntityRenderer {
public:
  typedef std::map<TexturedModelType, std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>> TexturedEntityMap;

  EntityRenderer();
  EntityRenderer(GLFWwindow* window, const StaticShader& program);

  void init(const glm::mat4& projMatrix);
  
  void render(const TexturedEntityMap& entities);
  
  void prepareTexturedModel(const TexturedModel& model);
  void unbindTexturedModel(const TexturedModel& model);
  
  void prepareInstance(std::shared_ptr<Entity> entity);
  
  void enableCulling();
  void disableCulling();
  void cleanUp();
  
  GLFWwindow* mWindowHdl;
  StaticShader mShaderProgram;
  Camera mCamera;
  glm::mat4 mProjectionMatrix;
};

#endif

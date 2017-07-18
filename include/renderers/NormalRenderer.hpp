#ifndef NormalRenderer_hpp
#define NormalRenderer_hpp

#include <OpenGL/gl3.h>
#include <OpenGl/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utilities.h"
#include "Camera.hpp"
#include "Light.hpp"
#include "NormalShader.hpp"

class NormalRenderer {
public:
  NormalShader mShaderProgram;
  Camera* mCameraHdl;
  glm::mat4 mProjectionMatrix;
  
  NormalRenderer();
  
  void init(const glm::mat4& projMatrix, Camera& camera);
  
  void render(const TexturedEntityMap& entities, const std::vector<Light>& lights, const glm::vec3& skyColor);
  
  void prepareTexturedModel(const TexturedModel& model);
  void unbindTexturedModel(const TexturedModel& model);

  void prepareInstance(std::shared_ptr<Entity> entity);
  void prepare(const std::vector<Light>& lights, const glm::vec3& skyColor);
  void cleanUp();
  
  void enableCulling();
  void disableCulling();
};

#endif

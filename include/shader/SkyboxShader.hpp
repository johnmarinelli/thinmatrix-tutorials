#ifndef SkyboxShader_hpp
#define SkyboxShader_hpp

#include "ShaderProgram.hpp"

const float SKYBOX_ROTATION_SPEED = 10.0f;
class SkyboxShader : public ShaderProgram {
public:
  
  SkyboxShader();
  
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void connectTextureUnits();
  
  void update(double dt);
  
  void loadBlendFactor(float bf);
  void loadFogColor(const glm::vec3& fogColor);
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
  void loadViewMatrix(const glm::mat4& viewMatrix);
  
  float mRotation;
};

#endif

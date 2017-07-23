#ifndef WaterShader_hpp
#define WaterShader_hpp

#include "ShaderProgram.hpp"
class Light;

class WaterShader : public ShaderProgram {
public:
  
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void connectTextureUnits();
  
  void loadLight(const Light& light);
  void loadLightColor(const glm::vec3& lightColor);
  void loadLightPosition(const glm::vec3& lightPosition);
  void loadMoveFactor(float moveFactor);
  void loadCameraPosition(const glm::vec3& cameraPosition);
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
  void loadViewMatrix(const glm::mat4& viewMatrix);  
  void loadModelMatrix(const glm::mat4& modelMatrix);
};

#endif 

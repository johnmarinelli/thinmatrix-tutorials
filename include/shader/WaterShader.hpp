#ifndef WaterShader_hpp
#define WaterShader_hpp

#include "ShaderProgram.hpp"

class WaterShader : public ShaderProgram {
public:
  
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void connectTextureUnits();
  
  void loadMoveFactor(float moveFactor);
  void loadCameraPosition(const glm::vec3& cameraPosition);
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
  void loadViewMatrix(const glm::mat4& viewMatrix);  
  void loadModelMatrix(const glm::mat4& modelMatrix);
};

#endif 

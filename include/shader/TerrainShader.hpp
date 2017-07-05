#ifndef TerrainShader_hpp
#define TerrainShader_hpp

#include <OpenGL/gl3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Light.hpp"
#include "ShaderProgram.hpp"

class TerrainShader : public ShaderProgram {
public:
  
  TerrainShader();
  
  void connectTextureUnits();
  
  void loadInt(GLuint i, const std::string& name);
  
  void loadFloat(GLfloat f, const std::string& name);
  
  void loadLight(const Light& light, const std::string& posName, const std::string& colName);
  
  void loadSkyColor(const glm::vec3& skyColor, const std::string& skyColorName);
  
  void loadShineVariables(float shineDamper, float reflectivity, const std::string& damperName, const std::string& refName);
  
  void loadAmbientFactor(float ambientFactor, const std::string& ambientName);
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix, const std::string& projectionMatrixName);
  
  void loadViewMatrix(const glm::mat4& viewMatrix, const std::string& viewMatrixName);
  
  void loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName);
};

#endif

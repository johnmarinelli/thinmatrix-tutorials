#ifndef TerrainShader_hpp
#define TerrainShader_hpp

#include <OpenGL/gl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Light.hpp"
#include "ShaderProgram.hpp"

class TerrainShader : public ShaderProgram {
public:
  
  TerrainShader();
  
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void connectTextureUnits();
  
  void loadInt(GLuint i, const std::string& name);
  
  void loadFloat(GLfloat f, const std::string& name);
  
  void loadLights(const std::vector<Light>& light);
  
  void loadSkyColor(const glm::vec3& skyColor);
  
  void loadShineVariables(float shineDamper, float reflectivity);
  
  void loadAmbientFactor(float ambientFactor);
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
  
  void loadViewMatrix(const glm::mat4& viewMatrix);
  
  void loadModelMatrix(const glm::mat4& modelMatrix);
};

#endif

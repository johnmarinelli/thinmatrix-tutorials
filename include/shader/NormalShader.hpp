#ifndef NormalShader_hpp
#define NormalShader_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "ShaderProgram.hpp"
#include "Light.hpp"

class NormalShader : public ShaderProgram {
public:
  NormalShader();
  
  void init(const std::string &vertexFilepath, const std::string &fragmentFilepath);
  
  void connectTextureUnits();
  void loadClipPlane(const glm::vec4& clipPlane);
  
  void loadNumTextureRows(float num);
  void loadTextureAtlasXYOffset(const glm::vec2& offset);
  
  void loadSkyColor(const glm::vec3& skyColor);
  void loadLights(const std::vector<Light>& lights, const glm::mat4& viewMatrix);
  void loadShineVariables(float damper, float reflectivity);
  
  void loadAmbientFactor(float ambientFactor);
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
  
  void loadViewMatrix(const glm::mat4& viewMatrix);
  
  void loadModelMatrix(const glm::mat4& modelMatrix);
  
  void loadInt(GLuint i, const std::string& name);
  
};

#endif

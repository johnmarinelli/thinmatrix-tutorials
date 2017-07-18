#ifndef StaticShader_hpp
#define StaticShader_hpp

#include <vector>

#include "ShaderProgram.hpp"

class Light;

class StaticShader : public ShaderProgram {
public:
  
  StaticShader();
  
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void loadTextureAtlasXYOffset(const glm::vec2& offset);
  void loadNumTextureRows(float num);
  void loadUseFakeLighting(bool useFake);
  void loadSkyColor(const glm::vec3& skyColor);
  void loadLights(const std::vector<Light>& lights);
  void loadShineVariables(float shineDamper, float reflectivity);
  void loadAmbientFactor(float ambientFactor);
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
  void loadViewMatrix(const glm::mat4& viewMatrix);
  void loadModelMatrix(const glm::mat4& modelMatrix);
  void loadHorizontalClipPlane(const glm::vec4& clipPlane);

};

#endif

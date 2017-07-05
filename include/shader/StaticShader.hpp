#ifndef StaticShader_hpp
#define StaticShader_hpp

#include <stdio.h>

#include "ShaderProgram.hpp"

class StaticShader : public ShaderProgram {
public:
  
  StaticShader();
  
  void loadTextureAtlasXYOffset(const glm::vec2& offset, const std::string& textureAtlasXYOffsetName);
  
  void loadNumTextureRows(float num, const std::string& numTextureRowsName);
  
  void loadUseFakeLighting(bool useFake, const std::string& useFakeLightingName);
  
  void loadSkyColor(const glm::vec3& skyColor, const std::string& skyColorName);
  
  void loadLight(const Light& light, const std::string& posName, const std::string& colName);
  
  void loadShineVariables(float shineDamper, float reflectivity, const std::string& damperName, const std::string& refName);
  
  void loadAmbientFactor(float ambientFactor, const std::string& ambientName);
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix, const std::string& projectionMatrixName);
  
  void loadViewMatrix(const glm::mat4& viewMatrix, const std::string& viewMatrixName);
  
  void loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName);

};

#endif

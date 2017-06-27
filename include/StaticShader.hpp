#ifndef StaticShader_hpp
#define StaticShader_hpp

#include <stdio.h>

#include "ShaderProgram.hpp"

class StaticShader : public ShaderProgram {
public:
  
  StaticShader() :
    ShaderProgram() {    
  }
  
  void loadLight(const Light& light, const std::string& posName, const std::string& colName) {
    auto lightPosLocation = mUniforms[posName],
    lightColLocation = mUniforms[colName];
    
    glUniform3f(lightPosLocation, light.mPosition.x, light.mPosition.y, light.mPosition.z);
    glUniform3f(lightColLocation, light.mColor.r, light.mColor.g, light.mColor.b);
  }
  
  void loadShineVariables(float shineDamper, float reflectivity, const std::string& damperName, const std::string& refName) {
    auto shineDamperLocation = mUniforms[damperName],
    reflectivityLocation = mUniforms[refName];
    
    glUniform1f(shineDamperLocation, shineDamper);
    glUniform1f(reflectivityLocation, reflectivity);
  }
  void loadAmbientFactor(float ambientFactor, const std::string& ambientName) {
    auto ambientFactorLocation = mUniforms[ambientName];
    
    glUniform1f(ambientFactorLocation, ambientFactor);
  }
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix, const std::string& projectionMatrixName) {
    auto projectionMatrixLocation = mUniforms[projectionMatrixName];
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  }
  
  void loadViewMatrix(const glm::mat4& viewMatrix, const std::string& viewMatrixName) {
    auto viewMatrixLocation = mUniforms[viewMatrixName];
    
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
  }
  
  void loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName) {
    auto modelMatrixLocation = mUniforms[modelMatrixName];
    
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));    
  }

};

#endif

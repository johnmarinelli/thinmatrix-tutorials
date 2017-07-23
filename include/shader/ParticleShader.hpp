#ifndef ParticleShader_hpp
#define ParticleShader_hpp

#include "ShaderProgram.hpp"

class ParticleShader : public ShaderProgram {
public:
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);

  void loadModelViewMatrix(const glm::mat4& modelViewMatrix);

};

#endif

#ifndef FontShader_hpp
#define FontShader_hpp

#include "ShaderProgram.hpp"

class FontShader : public ShaderProgram {
public:
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void loadProjectionMatrix(const glm::mat4& projectionMatrix);
};

#endif

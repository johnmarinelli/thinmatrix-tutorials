#ifndef GUIShader_hpp
#define GUIShader_hpp

#include <OpenGL/gl3.h>

#include "ShaderProgram.hpp"

class GUIShader : public ShaderProgram {
public:
  
  void init(const std::string& vertexFilepath, const std::string& fragmentFilepath);
  
  void loadModelMatrix(const glm::mat4& modelMatrix);
};

#endif

#ifndef GUIShader_hpp
#define GUIShader_hpp

#include <OpenGL/gl3.h>

#include "ShaderProgram.hpp"

class GUIShader : public ShaderProgram {
public:
  void loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName);
};

#endif

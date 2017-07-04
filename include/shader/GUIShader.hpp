#ifndef GUIShader_hpp
#define GUIShader_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>

#include "ShaderProgram.hpp"

class GUIShader : public ShaderProgram {
public:
  void loadModelMatrix(const glm::mat4& modelMatrix, const std::string& modelMatrixName) {
    auto modelMatrixLocation = mUniforms[modelMatrixName];
    
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
  }
};

#endif /* GUIShader_hpp */

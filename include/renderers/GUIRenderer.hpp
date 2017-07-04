#ifndef GUIRenderer_hpp
#define GUIRenderer_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "GUIShader.hpp"
#include "GUITexture.hpp"
#include "RawModel.hpp"
#include "Loader.hpp"

class GUIRenderer {
public:
  
  RawModel mQuad;
  GUIShader mShader;
  
  GUIRenderer(Loader& loader) {
    GLfloat positions[] = {
      -1.0f, 1.0f,
      -1.0f, -1.0f,
      1.0f, 1.0f,
      1.0f, -1.0f
    };
    mQuad = loader.loadQuadToVAO(positions);
    mShader = loadGUIShader();
  }
  
  GUIShader loadGUIShader() {
    GUIShader shaderProgram;
    shaderProgram.initFromFiles("resources/shaders/gui.vert", "resources/shaders/gui.frag");
    shaderProgram.registerUniform("modelMatrix");
    std::cout << glGetError() << std::endl;
    return shaderProgram;
  }
  
  void render(const std::vector<GUITexture>& guis) {
    mShader.use();
    glBindVertexArray(mQuad.mVaoID);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
    
    std::cout << glGetError() << std::endl;
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    //glDepthMask(false);
    
    for (auto gui : guis) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gui.mTextureID);
      
      glm::mat4 modelMatrix{1.0f};
      modelMatrix = glm::translate(modelMatrix, glm::vec3{gui.mPosition.x, gui.mPosition.y, 0.0});
      modelMatrix = glm::scale(modelMatrix, glm::vec3{gui.mScale.x, gui.mScale.y, 1.0});
      mShader.loadModelMatrix(modelMatrix, "modelMatrix");
      
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(true);
    //glDisable(GL_BLEND);
    
    glDisableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
    glBindVertexArray(0);
    mShader.disable();
   
  }
};

#endif

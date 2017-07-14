#include "GUIRenderer.hpp"

GUIRenderer::GUIRenderer(Loader& loader) {
  GLfloat positions[] = {
    -1.0f, 1.0f,
    -1.0f, -1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f
  };
  mQuad = loader.loadQuadToVAO(positions);
  mShaderProgram = loadGUIShader();
}

GUIShader GUIRenderer::loadGUIShader() {
  GUIShader shaderProgram;
  shaderProgram.init("resources/shaders/gui.vert", "resources/shaders/gui.frag");
  
  return shaderProgram;
}

void GUIRenderer::render(const std::vector<GUITexture>& guis) {
  mShaderProgram.use();
  glBindVertexArray(mQuad.mVaoID);
  glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
  
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
    mShaderProgram.loadModelMatrix(modelMatrix);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
  
  glEnable(GL_DEPTH_TEST);
  //glDepthMask(true);
  //glDisable(GL_BLEND);
  
  glDisableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
  glBindVertexArray(0);
  mShaderProgram.disable();
}

void GUIRenderer::cleanUp() {
  mShaderProgram.cleanUp();
}

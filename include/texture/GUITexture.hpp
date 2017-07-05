#ifndef GUITexture_hpp
#define GUITexture_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class GUITexture {
public:
  GUITexture(int texID, const glm::vec2& pos, const glm::vec2& scale);
  
  int mTextureID;
  
  // center of quad
  glm::vec2 mPosition;
  
  // x,y size of quad related to size of screen
  glm::vec2 mScale;
  
};

#endif

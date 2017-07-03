#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class Light {
public:
  glm::vec3 mPosition;
  glm::vec3 mColor;
  
  Light(const glm::vec3& pos, const glm::vec3& col) :
    mPosition(pos),
    mColor(col) {
    
  }
};

#endif

#include "Light.hpp"

Light::Light(const glm::vec3& pos, const glm::vec3& col) :
  mPosition(pos),
  mColor(col),
  mAttenuation(glm::vec3{1.0f, 0.0f, 0.0f}){
}

Light::Light(const glm::vec3& pos, const glm::vec3& col, const glm::vec3& attenuation) :
  mPosition(pos),
  mColor(col),
  mAttenuation(attenuation){
}

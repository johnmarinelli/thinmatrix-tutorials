#ifndef SkyboxRenderer_hpp
#define SkyboxRenderer_hpp

#include <vector>
#include <string>

#include "Loader.hpp"
#include "RawModel.hpp"
#include "SkyboxShader.hpp"

static const float SKYBOX_SIZE = 1.0f;

class Camera;

class SkyboxRenderer {
public:
  RawModel mCube;
  GLuint mDayTextureID;
  GLuint mNightTextureID;
  SkyboxShader mShaderProgram;
  double mElapsedTime;
  
  SkyboxRenderer();
  
  void init(Loader& loader, const glm::mat4& projectionMatrix);
  
  void update(double dt);
  void render(const Camera& camera, const glm::vec3& fogColor);
  
  void bindTextures();
  
};

#endif

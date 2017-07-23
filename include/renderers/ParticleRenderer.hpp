#ifndef ParticleRenderer_hpp
#define ParticleRenderer_hpp

#include "RawModel.hpp"
#include "ParticleShader.hpp"
#include "Particle.hpp"

class Loader;
class Camera;

class ParticleRenderer {
public:
  RawModel mQuad;
  ParticleShader mShaderProgram;
  Camera* mCameraHdl;
  glm::mat4 mProjectionMatrix;
  
  ParticleRenderer();
  void init(Loader& loader, const glm::mat4& projectionMatrix, Camera& camera);
  
  void render(const std::vector<Particle>& particles);
  void prepare();
  void updateModelViewMatrix(const glm::vec3& position, float rotation, float scale, glm::mat4& viewMatrix);
  void cleanUp();
  void finishRendering();
  
};

#endif

#ifndef WaterRenderer_hpp
#define WaterRenderer_hpp

#include <vector>
#include "WaterTile.hpp"
#include "WaterFrameBuffers.hpp"
#include "WaterShader.hpp"
#include "RawModel.hpp"

class Loader;
class Camera;

class WaterRenderer {
public:
  RawModel mQuad;
  WaterShader mShaderProgram;
  Camera* mCameraHdl;
  glm::mat4 mProjectionMatrix;
  WaterFrameBuffers* mWaterFramebuffersHdl;
  GLuint mDuDvTexture;
  const float WAVE_SPEED = 10.0f;
  float mMoveFactor;
  
  WaterRenderer();
  
  void init(Loader& loader, const glm::mat4& projectionMatrix, Camera& camera, WaterFrameBuffers& waterFBs);
  
  void update(double dt);
  void render(const std::vector<WaterTile>& water);
  void prepare();
  
  void unbind();
  void setupVAO(Loader& loader);
};

#endif

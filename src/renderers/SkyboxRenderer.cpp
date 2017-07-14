#include "SkyboxRenderer.hpp"
#include "Camera.hpp"

SkyboxRenderer::SkyboxRenderer() {
}

void SkyboxRenderer::init(Loader& loader, const glm::mat4& projectionMatrix) {
  mShaderProgram.init("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
  
  GLfloat SKYBOX_VERTICES[] = {
    -SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    
    -SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    
    -SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    
    -SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
    -SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
    
    -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE};
  
  mCube = loader.loadCubeToVAO(SKYBOX_VERTICES);
  
  std::vector<std::string> daySkyboxTextureFilepaths;
  daySkyboxTextureFilepaths.push_back("resources/textures/cubemaps/day/right.png");
  daySkyboxTextureFilepaths.push_back("resources/textures/cubemaps/day/left.png");
  daySkyboxTextureFilepaths.push_back("resources/textures/cubemaps/day/top.png");
  daySkyboxTextureFilepaths.push_back("resources/textures/cubemaps/day/bottom.png");
  daySkyboxTextureFilepaths.push_back("resources/textures/cubemaps/day/back.png");
  daySkyboxTextureFilepaths.push_back("resources/textures/cubemaps/day/front.png");
  
  mDayTextureID = loader.loadCubemap(daySkyboxTextureFilepaths);
  
  std::vector<std::string> nightSkyboxTextureFilepaths;
  nightSkyboxTextureFilepaths.push_back("resources/textures/cubemaps/night/right.png");
  nightSkyboxTextureFilepaths.push_back("resources/textures/cubemaps/night/left.png");
  nightSkyboxTextureFilepaths.push_back("resources/textures/cubemaps/night/top.png");
  nightSkyboxTextureFilepaths.push_back("resources/textures/cubemaps/night/bottom.png");
  nightSkyboxTextureFilepaths.push_back("resources/textures/cubemaps/night/back.png");
  nightSkyboxTextureFilepaths.push_back("resources/textures/cubemaps/night/front.png");
  
  mNightTextureID = loader.loadCubemap(nightSkyboxTextureFilepaths);
  
  mShaderProgram.use();
  mShaderProgram.loadProjectionMatrix(projectionMatrix);
  mShaderProgram.disable();
}

void SkyboxRenderer::update(double dt) {
  mElapsedTime += dt * 1000.0;
  mShaderProgram.update(dt);
}

void SkyboxRenderer::render(const Camera& camera, const glm::vec3& fogColor) {
  // set depth function to <= for depth test optimization
  glDepthFunc(GL_LEQUAL);
  mShaderProgram.use();
  mShaderProgram.loadFogColor(fogColor);
  mShaderProgram.loadViewMatrix(camera.createViewMatrix());
  glBindVertexArray(mCube.mVaoID);
  glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
  bindTextures();
  glDrawArrays(GL_TRIANGLES, 0, mCube.mVertexCount);
  glDisableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
  glBindVertexArray(0);
  
  mShaderProgram.disable();
  glDepthFunc(GL_LESS);
}

void SkyboxRenderer::bindTextures() {
  
  auto time = fmod(mElapsedTime, 24.0);
  
  GLuint tex1, tex2;
  float blend = 0.0f;
  
  if (time >= 0.0 && time < 5.0) {
    tex1 = mNightTextureID;
    tex2 = mNightTextureID;
    blend = time / 5.0;
  }
  
  else if (time >= 5.0 && time < 8.0) {
    tex1 = mNightTextureID;
    tex2 = mDayTextureID;
    blend = (time - 5.0) / 3.0;
  }
  else if (time >= 8.0 && time < 21.0) {
    tex1 = mDayTextureID;
    tex2 = mDayTextureID;
    blend = (time - 8.0) / 16.0;
  }
  else {
    tex1 = mDayTextureID;
    tex2 = mNightTextureID;
    blend = (time - 21.0) / 3.0;
  }
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, tex1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, tex2);
  
  mShaderProgram.loadBlendFactor(blend);
}

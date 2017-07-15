#ifndef MasterRenderer_hpp
#define MasterRenderer_hpp

#include <GLFW/glfw3.h>
#include <map>

#include "StaticShader.hpp"
#include "EntityRenderer.hpp"
#include "TerrainShader.hpp"
#include "TerrainRenderer.hpp"
#include "NormalShader.hpp"
#include "NormalRenderer.hpp"
#include "TexturedModel.hpp"
#include "SkyboxRenderer.hpp"
#include "Entity.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class Loader;

class MasterRenderer {
public:
  MasterRenderer();
  
  void init(GLFWwindow* window, Loader& loader);
  
  void addTexturedModel(const TexturedModel& texturedModel);
  void addNormalTexturedModel(const TexturedModel& texturedModel);
  
  void addEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType);
  void addNormalEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType);
  
  void addTerrain(const Terrain& terrain);
  void update(double dt);
  void render(const std::vector<Light>& lights);
  
  void prepare();
  glm::mat4 createProjectionMatrix(int width, int height) const;
  void cleanUp();
  
  StaticShader mStaticShader;
  TerrainShader mTerrainShader;
  SkyboxShader mSkyboxShader;
  NormalShader mNormalShader;
  EntityRenderer mEntityRenderer;
  TerrainRenderer mTerrainRenderer;
  SkyboxRenderer mSkyboxRenderer;
  NormalRenderer mNormalRenderer;
  
  std::vector<Terrain> mTerrains;
  glm::mat4 mProjectionMatrix;
  int mRenderWidth, mRenderHeight;
  
  glm::vec3 mSkyColor;
  
  TexturedEntityMap mEntities;
  TexturedEntityMap mNormalEntities;
};

#endif

#ifndef MasterRenderer_hpp
#define MasterRenderer_hpp

#include <stdio.h>
#include <map>

#include "StaticShader.hpp"
#include "EntityRenderer.hpp"
#include "TerrainShader.hpp"
#include "TerrainRenderer.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class MasterRenderer {
public:
  MasterRenderer();
  
  MasterRenderer(GLFWwindow*, const StaticShader&, const TerrainShader&);
  
  void init();
  
  void addTexturedModel(const TexturedModel& texturedModel);
  
  void addEntity(std::shared_ptr<Entity> entity, TexturedModelType texturedModelType);
  
  void addTerrain(const Terrain& terrain);
  void render(const Light& sun);
  
  void prepare();
  glm::mat4 createProjectionMatrix(int width, int height) const;
  void cleanUp();
  
  StaticShader mShaderProgram;
  TerrainShader mTerrainShader;
  EntityRenderer mEntityRenderer;
  TerrainRenderer mTerrainRenderer;
  
  std::vector<Terrain> mTerrains;
  GLFWwindow* mWindowHdl;
  glm::mat4 mProjectionMatrix;
  int mRenderWidth, mRenderHeight;
  
  glm::vec3 mSkyColor;
  
  std::map<TexturedModelType, std::pair<TexturedModel, std::vector<std::shared_ptr<Entity>>>> mEntities;
};

#endif

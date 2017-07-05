#ifndef TerrainRenderer_hpp
#define TerrainRenderer_hpp

#include <glm/glm.hpp>
#include <vector>

#include "TerrainShader.hpp"

class TerrainShader;
class GLFWwindow;
class Terrain;

class TerrainRenderer {
public:
  TerrainRenderer();
  TerrainRenderer(GLFWwindow* window, const TerrainShader& shader);
  
  void init(const glm::mat4& projMatrix);
  void render(const std::vector<Terrain>& terrains);
  void prepareTerrain(const Terrain& terrain);
  void bindTextures(const Terrain& terrain);
  void unbindTexturedModel(const Terrain& terrain);
  void prepareInstance(const Terrain& terrain);
  
  void cleanUp();
  
  TerrainShader mShaderProgram;
  GLFWwindow* mWindowHdl;
  glm::mat4 mProjectionMatrix;
};

#endif 

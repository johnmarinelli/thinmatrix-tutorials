#ifndef GUIRenderer_hpp
#define GUIRenderer_hpp

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "GUIShader.hpp"
#include "GUITexture.hpp"
#include "RawModel.hpp"
#include "Loader.hpp"

class GUIRenderer {
public:
  GUIRenderer(Loader&);
  GUIShader loadGUIShader();
  
  void render(const std::vector<GUITexture>& guis);
  void cleanUp();
  
  RawModel mQuad;
  GUIShader mShaderProgram;
  
};

#endif

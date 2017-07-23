#ifndef FontRenderer_hpp
#define FontRenderer_hpp

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "FontShader.hpp"

class FontRenderer {
public:
  FT_Library mFont;
  FT_Face mFontFace;
  FontShader mShaderProgram;
  FT_GlyphSlot mGlyph;
  GLuint mTextureID;
  int mFontSize;
  float mAtlasWidth, mAtlasHeight;
  
  FontRenderer();
  void init();
  void createCharacters();
  
  void render(const std::string& text, const glm::mat4& projectionMatrix);
  
  void renderText(const std::string& text, float x, float y);
  
  ~FontRenderer();
  
  struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
  };
  
  std::map<GLchar, Character> mCharacters;

  GLuint mVBO, mVAO;
};

#endif

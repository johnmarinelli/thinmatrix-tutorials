#include "FontRenderer.hpp"
#include "Utilities.h"
#include <iostream>
#include <algorithm>

FontRenderer::FontRenderer() :
  mFontSize(48),
  mShaderProgram() {
  init();
}

void FontRenderer::init() {
  if (FT_Init_FreeType(&mFont)) {
    std::cout << "Could not init freetype library\n";
    throw std::runtime_error("Could not init freetype library\n");
  }
  
  if (FT_New_Face(mFont, "resources/fonts/Courier.ttf", 0, &mFontFace)) {
    std::cout << "Could not open freetype font\n";
    throw std::runtime_error("Could not open freetype font\n");
  }
  
  FT_Set_Pixel_Sizes(mFontFace, 0, 48);
  
  mGlyph = mFontFace->glyph;
  
  mShaderProgram.init("resources/shaders/font.vert", "resources/shaders/font.frag");
  createCharacters();
}

/* 
 * TODO: use texture atlases and SDF rendering
 */
void FontRenderer::createCharacters() {

  int width = 0;
  int height = 0;
  
  // save unpack alignment for resetting later
  GLint originalUnpackAlignment;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &originalUnpackAlignment);
  
  // disable byte alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  for (GLubyte c = 0; c < 128; ++c) {
    // Load character glyph
    if (FT_Load_Char(mFontFace, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 mGlyph->bitmap.width,
                 mGlyph->bitmap.rows,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 mGlyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Character character = {
      texID,
      glm::ivec2(mGlyph->bitmap.width, mGlyph->bitmap.rows),
      glm::ivec2(mGlyph->bitmap_left, mGlyph->bitmap_top),
      (GLuint) mGlyph->advance.x
    };
    
    mCharacters.insert(std::pair<GLchar, Character>(c, character));
    
    width += mGlyph->bitmap.width;
    height = std::max(height, (int) mGlyph->bitmap.rows);
  }
  
  mAtlasWidth = width;
  mAtlasHeight = height;
  
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  // reset unpacking alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, originalUnpackAlignment);
}

void FontRenderer::render(const std::string& text, const glm::mat4& projectionMatrix) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  mShaderProgram.use();
  glBindVertexArray(mVAO);
  mShaderProgram.loadProjectionMatrix(projectionMatrix);
  glActiveTexture(GL_TEXTURE0);
  
  renderText(text, 250.0f, 25.0f);
  
  mShaderProgram.disable();
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
}

void FontRenderer::renderText(const std::string& text, float x, float y) {
  std::string::const_iterator sitr;
  
  float scale = 1.0;
  
  for (sitr = text.begin(); sitr != text.end(); ++sitr) {

    Character c = mCharacters[(GLchar) *sitr];
    GLfloat xpos = x + c.bearing.x * scale;
    GLfloat ypos = y - (c.size.y - c.bearing.y) * scale;
    GLfloat w = c.size.x * scale;
    GLfloat h = c.size.y * scale;
    GLfloat vertices[6][4] = {
      { xpos,     ypos + h,   0.0, 0.0 },
      { xpos,     ypos,       0.0, 1.0 },
      { xpos + w, ypos,       1.0, 1.0 },
      
      { xpos,     ypos + h,   0.0, 0.0 },
      { xpos + w, ypos,       1.0, 1.0 },
      { xpos + w, ypos + h,   1.0, 0.0 }
    };
    
    glBindTexture(GL_TEXTURE_2D, c.textureID);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    x += (c.advance >> 6) * scale;
  }
}

FontRenderer::~FontRenderer() {  
  FT_Done_Face(mFontFace);
  FT_Done_FreeType(mFont);
}

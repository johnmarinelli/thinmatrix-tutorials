#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <OpenGL/gl3.h>

#include "ShaderProgram.hpp"
#include "Utilities.h"

struct Vertex {
  glm::vec3 mPosition;
  glm::vec3 mNormal;
  glm::vec2 mTexCoord;
};

struct ObjTexture {
  GLuint mID;
  std::string mType;
};

class Mesh {
public:
  std::vector<Vertex> mVertices;
  std::vector<GLuint> mIndices;
  std::vector<ObjTexture> mTextures;
  GLuint mVAO, mVBO, mIBO;
  
  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<ObjTexture> textures) :
    mVertices(vertices),
    mIndices(indices),
    mTextures(textures) {
    setupMesh();
  }
  
  void draw(const ShaderProgram& shaderProgram) {
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
  
  void setupMesh() {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mIBO);
    
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
    glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    
    glEnableVertexAttribArray(NORMAL_ATTRIBUTE_INDEX);
    glVertexAttribPointer(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, mNormal));
    
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIBUTE_INDEX);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, mTexCoord));
    
    glBindVertexArray(0);
  }
  
  
};

#endif

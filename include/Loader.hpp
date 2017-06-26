#ifndef Loader_hpp
#define Loader_hpp

#include <stdio.h>
#include <vector>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <string>

#include "RawModel.hpp"
#include "Utilities.h"
#include "Texture.hpp"

class Loader {
    
public:
  std::vector<GLuint> mVAOs = {};
  std::vector<GLuint> mVBOs = {};
  std::vector<GLuint> mTextures = {};
  
  /*RawModel loadToVAO(GLfloat positions[], GLuint indices[], GLfloat texCoords[], GLuint numVertices, GLuint numIndices, GLuint numTextureCoords) {
    GLuint vaoID = createVAO();
    bindIndicesBuffer(indices, numIndices);
    
    storeDataInAttributeList(POSITION_ATTRIBUTE_INDEX, positions, 3, numVertices);
    storeDataInAttributeList(TEXTURE_COORD_ATTRIBUTE_INDEX, texCoords, 2, numTextureCoords);
    
    unbindVAO();
    
    RawModel model{vaoID, numVertices, numIndices};
    model.mAttributes.push_back(POSITION_ATTRIBUTE_INDEX);
    model.mAttributes.push_back(TEXTURE_COORD_ATTRIBUTE_INDEX);
    
    return model;
  }*/
  
  RawModel loadToVAO(GLfloat positions[],
                     GLuint indices[],
                     GLfloat texCoords[],
                     GLfloat normals[],
                     GLuint numVertices,
                     GLuint numIndices,
                     GLuint numTextureCoords,
                     GLuint numNormals) {
    GLuint vaoID = createVAO();
    bindIndicesBuffer(indices, numIndices);
    
    storeDataInAttributeList(POSITION_ATTRIBUTE_INDEX, positions, 3, numVertices);
    storeDataInAttributeList(NORMAL_ATTRIBUTE_INDEX, normals, 3, numNormals);
    storeDataInAttributeList(TEXTURE_COORD_ATTRIBUTE_INDEX, texCoords, 2, numTextureCoords);
    unbindVAO();
    
    RawModel model{vaoID, numVertices, numIndices};
    model.mAttributes.push_back(POSITION_ATTRIBUTE_INDEX);
    model.mAttributes.push_back(NORMAL_ATTRIBUTE_INDEX);
    model.mAttributes.push_back(TEXTURE_COORD_ATTRIBUTE_INDEX);
    
    return model;
  }
  
  GLuint createVAO() {
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    mVAOs.push_back(vaoID);
    return vaoID;
  }
  
  void storeDataInAttributeList(int attNum, GLfloat data[], int coordSize, GLuint numVertices) {
    GLuint vboID;
    glGenBuffers(1, &vboID);
    mVBOs.push_back(vboID);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    GLfloat* buf = &data[0];
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), buf, GL_STATIC_DRAW);
    
    // specifies all the state that opengl needs to know about for attribute `attNum`
    // it has `coordSize` compponents
    // it is of type GL_FLOAT
    // it is not normalized
    // it has a stride of 0
    // it starts at byte 0 of buffer
    glVertexAttribPointer(attNum, coordSize, GL_FLOAT, false, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  
  void bindIndicesBuffer(GLuint indices[], GLuint numIndices) {
    GLuint vboID;
    glGenBuffers(1, &vboID);
    mVBOs.push_back(vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
    
    GLuint* buf = &indices[0];
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), buf, GL_STATIC_DRAW);
  }
  
  GLint loadTexture(std::string filename) {
    Texture texture{filename};
    GLint texID = texture.mTextureID;
    mTextures.push_back(texID);
    return texID;
  }
  
  void unbindVAO() {
    glBindVertexArray(0);
  }
  
  void cleanUp() {
    for (auto vao : mVAOs) {
      glDeleteVertexArrays(1, &vao);
    }
    for (auto vbo : mVBOs) {
      glDeleteBuffers(1, &vbo);
    }
    for (auto tex : mTextures) {
      glDeleteTextures(1, &tex);
    }
    mVBOs.clear();
    mVAOs.clear();
    mTextures.clear();
  }
};

#endif

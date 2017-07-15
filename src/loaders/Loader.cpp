#include "Loader.hpp"
#include "Utilities.h"
#include "Texture.hpp"
#include "stb_image.h"
#include <iostream>

RawModel Loader::loadToVAO(GLfloat positions[],
                   GLfloat normals[],
                   GLfloat texCoords[],
                   GLuint indices[],
                   int numVertices,
                   int numNormals,
                   int numTextureCoords,
                   int numIndices) {
  GLuint vaoID = createVAO();
  bindIndicesBuffer(indices, numIndices);
  
  storeDataInAttributeList(POSITION_ATTRIBUTE_INDEX, positions, 3, numVertices);
  storeDataInAttributeList(NORMAL_ATTRIBUTE_INDEX, normals, 3, numNormals);
  storeDataInAttributeList(TEXTURE_COORD_ATTRIBUTE_INDEX, texCoords, 2, numTextureCoords);
  unbindVAO();
  
  RawModel model{vaoID, (GLuint) numVertices, (GLuint) numIndices};
  model.mAttributes.push_back(POSITION_ATTRIBUTE_INDEX);
  model.mAttributes.push_back(NORMAL_ATTRIBUTE_INDEX);
  model.mAttributes.push_back(TEXTURE_COORD_ATTRIBUTE_INDEX);
  
  return model;
}

RawModel Loader::loadToVAO(GLfloat positions[],
                           GLfloat normals[],
                           GLfloat texCoords[],
                           GLfloat tangents[],
                           GLuint indices[],
                           int numVertices,
                           int numNormals,
                           int numTextureCoords,
                           int numTangents,
                           int numIndices) {
  GLuint vaoID = createVAO();
  bindIndicesBuffer(indices, numIndices);
  
  storeDataInAttributeList(POSITION_ATTRIBUTE_INDEX, positions, 3, numVertices);
  storeDataInAttributeList(NORMAL_ATTRIBUTE_INDEX, normals, 3, numNormals);
  storeDataInAttributeList(TEXTURE_COORD_ATTRIBUTE_INDEX, texCoords, 2, numTextureCoords);
  storeDataInAttributeList(TANGENT_ATTRIBUTE_INDEX, tangents, 3, numTangents);
  unbindVAO();
  
  RawModel model{vaoID, (GLuint) numVertices, (GLuint) numIndices};
  model.mAttributes.push_back(POSITION_ATTRIBUTE_INDEX);
  model.mAttributes.push_back(NORMAL_ATTRIBUTE_INDEX);
  model.mAttributes.push_back(TEXTURE_COORD_ATTRIBUTE_INDEX);
  model.mAttributes.push_back(TANGENT_ATTRIBUTE_INDEX);
  
  return model;
}

RawModel Loader::loadQuadToVAO(GLfloat positions[]) {
  GLuint vaoID = createVAO();
  
  GLuint vboID;
  glGenBuffers(1, &vboID);
  mVBOs.push_back(vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  GLfloat* buf = &positions[0];
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), buf, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  unbindVAO();
  
  RawModel model{vaoID, 4, 0};
  model.mAttributes.push_back(POSITION_ATTRIBUTE_INDEX);
  
  return model;
}

RawModel Loader::loadCubeToVAO(GLfloat positions[]) {
  GLuint vaoID = createVAO();
  
  GLuint vboID;
  glGenBuffers(1, &vboID);
  mVBOs.push_back(vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  GLfloat* buf = &positions[0];
  glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), buf, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  unbindVAO();
  
  RawModel model{vaoID, 36, 0};
  model.mAttributes.push_back(POSITION_ATTRIBUTE_INDEX);
  
  return model;
}

GLuint Loader::createVAO() {
  GLuint vaoID;
  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);
  mVAOs.push_back(vaoID);
  return vaoID;
}

void Loader::storeDataInAttributeList(int attNum, GLfloat data[], int coordSize, int dataSize) {
  GLuint vboID;
  glGenBuffers(1, &vboID);
  mVBOs.push_back(vboID);
  
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  GLfloat* buf = &data[0];
  glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(GLfloat), buf, GL_STATIC_DRAW);
  
  // specifies all the state that opengl needs to know about for attribute `attNum`
  // it has `coordSize` compponents
  // it is of type GL_FLOAT
  // it is not normalized
  // it has a stride of 0
  // it starts at byte 0 of buffer
  glVertexAttribPointer(attNum, coordSize, GL_FLOAT, false, 0, 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIndicesBuffer(GLuint indices[], GLuint numIndices) {
  GLuint vboID;
  glGenBuffers(1, &vboID);
  mVBOs.push_back(vboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
  
  GLuint* buf = &indices[0];
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), buf, GL_STATIC_DRAW);
}

GLint Loader::loadTexture(const std::string& filename) {
  Texture texture{filename};
  GLint texID = texture.mTextureID;
  mTextures.push_back(texID);
  return texID;
}

GLuint Loader::loadCubemap(const std::vector<std::string>& filepaths) {
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
  
  int width, height, numChannels;
  
  for (int i = 0; i < filepaths.size(); ++i) {
    auto path = filepaths[i];
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }
    else {
      std::cout << "Cubemap texture failed to load at path " << path << '\n';
      stbi_image_free(data);
    }
  }
  
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  
  return texID;
}

/*GLuint Loader::loadCubeMap(const std::vector<std::string>& facesFilepaths) {
  GLuint texID = 0;
  glGenTextures(1, &texID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
  
  int width, height;
  
  // remember to put filepaths IN ORDER of GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, etc.
  for (int i = 0; i < facesFilepaths.size(); ++i) {
    png_bytep* data = readPNG(facesFilepaths.at(i), width, height);
    if (nullptr != data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      
    }
    else {
      std::cout << "Cubemap texture failed to load: " << facesFilepaths.at(i) << '\n';
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    free(data);
  }
  
  mTextures.push_back(texID);
  
  return texID;
}
 */

void Loader::unbindVAO() {
  glBindVertexArray(0);
}

void Loader::cleanUp() {
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

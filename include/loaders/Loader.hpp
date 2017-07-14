#ifndef Loader_hpp
#define Loader_hpp

#include <stdio.h>
#include <vector>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <string>

#include "RawModel.hpp"

class Loader {
    
public:
  std::vector<GLuint> mVAOs = {};
  std::vector<GLuint> mVBOs = {};
  std::vector<GLuint> mTextures = {};
  
  RawModel loadToVAO(GLfloat positions[],
                     GLfloat normals[],
                     GLfloat texCoords[],
                     GLuint indices[],
                     int numVertices,
                     int numNormals,
                     int numTextureCoords,
                     int numIndices);
  
  RawModel loadQuadToVAO(GLfloat positions[]);
  RawModel loadCubeToVAO(GLfloat positions[]);
  
  GLuint createVAO();
  
  void storeDataInAttributeList(int attNum, GLfloat data[], int coordSize, int count);
  
  void bindIndicesBuffer(GLuint indices[], GLuint numIndices);
  
  GLint loadTexture(const std::string& filename);
  
  GLuint loadCubemap(const std::vector<std::string>& facesFilepaths);
  
  void unbindVAO();
  void cleanUp();
};

#endif

#ifndef NormalObjLoader_hpp
#define NormalObjLoader_hpp

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Vertex.hpp"
#include "VertexNM.hpp"
#include "ModelData.hpp"
#include "ModelDataNM.hpp"

class NormalObjLoader {
public:
  
  ModelDataNM loadObj(const std::string& filename);
  
  float findFurthestPoint(const std::vector<VertexNM>& vertices);
  
  VertexNM* processVertex(const std::vector<std::string>& vtxData, std::vector<VertexNM>& vertices, std::vector<GLuint>& indices);
  
  void calculateTangents(VertexNM* v0, VertexNM* v1, VertexNM* v2, const std::vector<glm::vec2>& uvs);
  
  VertexNM* handleProcessedVertex(VertexNM* previousVertexNM, GLuint newTextureIndex, GLuint newNormalIndex, std::vector<GLuint>& indices, std::vector<VertexNM>& vertices);
  
  void removeUnusedVertices(std::vector<VertexNM>& vertices);

  //void processVertex(const std::vector<std::string>& vtxData, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
  
  //void handleProcessedVertex(Vertex* previousVertexNM, GLuint newTextureIndex, GLuint newNormalIndex, std::vector<GLuint>& indices, std::vector<Vertex>& vertices);
  
  //void removeUnusedVertices(std::vector<Vertex>& vertices);
  //float findFurthestPoint(const std::vector<Vertex>& vertices);

  
  std::string readObjFile(const std::string& filename);
  
  std::vector<std::string> splitLineBySpace(const std::string& line);
  
  std::vector<std::string> splitLine(const std::string& line, char delim);
  
  bool lineStartsWith(const std::string& line, const std::string& subStr);
  
  glm::vec2 parsePair(const std::vector<std::string>& line);
  
  glm::vec3 parseTriplet(const std::vector<std::string>& line);
  
  std::vector<GLfloat> mVerticesVector;
  std::vector<GLfloat> mNormalsVector;
  std::vector<GLfloat> mTexturesVector;
  std::vector<GLuint> mIndicesVector;
};

#endif

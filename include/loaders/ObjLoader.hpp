#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Vertex.hpp"
#include "ModelData.hpp"
#include "RawModel.hpp"
#include "Loader.hpp"

class ObjLoader {
public:
  
  ModelData loadObj(const std::string& filename);
  
  float findFurthestPoint(const std::vector<Vertex>& vertices);
  
  void processVertex(const std::vector<std::string>& vtxData, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
  
  void handleProcessedVertex(Vertex* previousVertex, GLuint newTextureIndex, GLuint newNormalIndex, std::vector<GLuint>& indices, std::vector<Vertex>& vertices);
  
  void removeUnusedVertices(std::vector<Vertex>& vertices);
  
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

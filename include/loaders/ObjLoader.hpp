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
  std::vector<GLfloat> mVerticesVector;
  std::vector<GLfloat> mNormalsVector;
  std::vector<GLfloat> mTexturesVector;
  std::vector<GLuint> mIndicesVector;
  
  ModelData loadObj(const std::string& filename) {
    std::ifstream file(filename.c_str());
    
    std::vector<Vertex> vertices;
    std::vector<GLfloat> verticesFlattened;
    
    std::vector<glm::vec2> textures;
    std::vector<GLfloat> texturesFlattened;
    
    std::vector<glm::vec3> normals;
    std::vector<GLfloat> normalsFlattened;
    
    std::vector<GLuint> indices;
    
    std::string lineBeforeBreak;
    
    while (true) {
      std::string line;
      std::getline(file, line);
      
      std::vector<std::string> currentLine = splitLineBySpace(line);
      if (lineStartsWith(line, "v ")) {
        int index = (int) vertices.size();
        Vertex vtx{index, parseTriplet(currentLine)};
        vertices.push_back(vtx);
      }
      else if (lineStartsWith(line, "vt ")) {
        glm::vec2 tx = parsePair(currentLine);
        textures.push_back(tx);
      }
      else if (lineStartsWith(line, "vn ")) {
        glm::vec3 vn = parseTriplet(currentLine);
        normals.push_back(vn);
      }
      else if (lineStartsWith(line, "f ")) {
        // need to seek back one line to get first 'f' value
        lineBeforeBreak = line;
        mTexturesVector.resize(vertices.size() * 2);
        mNormalsVector.resize(vertices.size() * 3);
        break;
      }
      else {
        continue;
      }
    }
    
    // process last line before break
    std::vector<std::string> currentLine = splitLineBySpace(lineBeforeBreak);
    std::vector<std::string> vtx1Str = splitLine(currentLine[1], '/');
    std::vector<std::string> vtx2Str = splitLine(currentLine[2], '/');
    std::vector<std::string> vtx3Str = splitLine(currentLine[3], '/');
    
    processVertex(vtx1Str, vertices, indices);
    processVertex(vtx2Str, vertices, indices);
    processVertex(vtx3Str, vertices, indices);
    
    for (std::string line; std::getline(file, line); ) {
      if (!lineStartsWith(line, "f ")) {
        continue;
      }
      std::vector<std::string> currentLine = splitLineBySpace(line);
      
      std::vector<std::string> vtx1Str = splitLine(currentLine[1], '/');
      std::vector<std::string> vtx2Str = splitLine(currentLine[2], '/');
      std::vector<std::string> vtx3Str = splitLine(currentLine[3], '/');
      
      processVertex(vtx1Str, vertices, indices);
      processVertex(vtx2Str, vertices, indices);
      processVertex(vtx3Str, vertices, indices);
    }
    
    file.close();
    removeUnusedVertices(vertices);
    
    verticesFlattened.resize(vertices.size() * 3);
    texturesFlattened.resize(vertices.size() * 2);
    normalsFlattened.resize(vertices.size() * 3);
    
    for (auto i = 0; i < vertices.size(); ++i) {
      auto vertex = vertices[i];
      auto position = vertex.mPosition;
      auto texCoord = textures[vertex.mTextureIndex];
      auto normal = normals[vertex.mNormalIndex];
      
      verticesFlattened[i * 3] = position.x;
      verticesFlattened[i * 3 + 1] = position.y;
      verticesFlattened[i * 3 + 2] = position.z;
      
      texturesFlattened[i * 2] = texCoord.x;
      texturesFlattened[i * 2 + 1] = texCoord.y;
      
      normalsFlattened[i * 3] = normal.x;
      normalsFlattened[i * 3 + 1] = normal.y;
      normalsFlattened[i * 3 + 2] = normal.z;
    }
    
    float furthest = findFurthestPoint(vertices);
    
    return ModelData{verticesFlattened, texturesFlattened, normalsFlattened, indices, furthest};
  }

  
  float findFurthestPoint(const std::vector<Vertex>& vertices) {
    float furthestPoint = std::numeric_limits<float>::min();
    for (auto v : vertices) {
      if (v.mLength > furthestPoint) furthestPoint = v.mLength;
    }
    return furthestPoint;
  }
  
  void processVertex(const std::vector<std::string>& vtxData, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    GLuint currentVertexIndex = (GLuint) std::atoi(vtxData[0].c_str()) - 1;
    GLuint currentTextureIndex = (GLuint) std::atoi(vtxData[1].c_str()) - 1;
    GLuint currentNormalIndex = (GLuint) std::atoi(vtxData[2].c_str()) - 1;
    
    Vertex* currentVertex = &vertices.at(currentVertexIndex);
    
    if (!currentVertex->isSet()) {
      currentVertex->mTextureIndex = currentTextureIndex;
      currentVertex->mNormalIndex = currentNormalIndex;
      indices.push_back(currentVertexIndex);
    }
    else {
      handleProcessedVertex(currentVertex, currentTextureIndex, currentNormalIndex, indices, vertices);
    }
  }
  
  void handleProcessedVertex(Vertex* previousVertex, GLuint newTextureIndex, GLuint newNormalIndex, std::vector<GLuint>& indices, std::vector<Vertex>& vertices) {
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
      indices.push_back(previousVertex->mIndex);
    }
    else {
      std::shared_ptr<Vertex> anotherVertex = previousVertex->mDuplicate;
      if (nullptr != anotherVertex) {
        handleProcessedVertex(anotherVertex.get(), newTextureIndex, newNormalIndex, indices, vertices);
      }
      else {
        std::shared_ptr<Vertex> duplicateVertex = std::make_shared<Vertex>(vertices.size(), previousVertex->mPosition);
        duplicateVertex->mTextureIndex = newTextureIndex;
        duplicateVertex->mNormalIndex = newNormalIndex;
        previousVertex->mDuplicate = duplicateVertex;
        vertices.push_back(*duplicateVertex);
        indices.push_back(duplicateVertex->mIndex);
      }
    }
  }
  
  void removeUnusedVertices(std::vector<Vertex>& vertices) {
    for (auto v : vertices) {
      if (!v.isSet()) {
        v.mTextureIndex = 0;
        v.mNormalIndex = 0;
      }
    }
  }
  
  std::string readObjFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    
    if (!file.good()) {
      throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    return stream.str();
  }
  
  std::vector<std::string> splitLineBySpace(const std::string& line) {
    return splitLine(line, ' ');
  }
  
  std::vector<std::string> splitLine(const std::string& line, char delim) {
    std::stringstream ss;
    ss.str(line);
    std::string item;
    std::vector<std::string> strs;
    while (std::getline(ss, item, delim)) {
      strs.push_back(item);
    }
    return strs;
  }
  
  bool lineStartsWith(const std::string& line, const std::string& subStr) {
    return 0 == std::strncmp(line.c_str(), subStr.c_str(), std::strlen(subStr.c_str()));
  }

  glm::vec2 parsePair(const std::vector<std::string>& line) {
    return glm::vec2{std::atof(line[1].c_str()), std::atof(line[2].c_str())};
  }
  
  glm::vec3 parseTriplet(const std::vector<std::string>& line) {
    return glm::vec3{std::atof(line[1].c_str()), std::atof(line[2].c_str()), std::atof(line[3].c_str())};
  }
};

#endif

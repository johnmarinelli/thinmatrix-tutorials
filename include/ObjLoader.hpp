#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "RawModel.hpp"
#include "Loader.hpp"

class ObjLoader {
public:
  std::vector<GLfloat> mVerticesVector;
  std::vector<GLfloat> mNormalsVector;
  std::vector<GLfloat> mTexturesVector;
  std::vector<GLuint> mIndicesVector;
  
  RawModel loadObjModel(const std::string& filename, Loader& loader) {
    std::ifstream file(filename.c_str());
    
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> textures;
    std::vector<GLuint> indices;
    
    std::string lineBeforeBreak;
    
    while (true) {
      std::string line;
      std::getline(file, line);
      
      std::vector<std::string> currentLine = splitLineBySpace(line);
      
      if (lineStartsWith(line, "v ")) {
        glm::vec3 vtx = parseTriplet(currentLine);
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
    
    mVerticesVector.resize(vertices.size() * 3);
    
    int vertexPointer = 0;
    
    for (auto vtx : vertices) {
      mVerticesVector[vertexPointer++] = vtx.x;
      mVerticesVector[vertexPointer++] = vtx.y;
      mVerticesVector[vertexPointer++] = vtx.z;
    }
    
    // example face structure:
    // f 41/1/1/ 38/2/1 45/3/1
    // each triplet: geometric vertex/texture vertex/vertex normal
    // 3 triplets = 1 face (triangle)
    std::vector<std::string> vtx1Str = splitLine(currentLine[1], '/');
    std::vector<std::string> vtx2Str = splitLine(currentLine[2], '/');
    std::vector<std::string> vtx3Str = splitLine(currentLine[3], '/');
    
    processVertex(vtx1Str, indices, textures, normals, mTexturesVector, mNormalsVector);
    processVertex(vtx2Str, indices, textures, normals, mTexturesVector, mNormalsVector);
    processVertex(vtx3Str, indices, textures, normals, mTexturesVector, mNormalsVector);
    
    for (std::string line; std::getline(file, line); ) {
      if (!lineStartsWith(line, "f ")) {
        continue;
      }
      std::vector<std::string> currentLine = splitLineBySpace(line);
      
      std::vector<std::string> vtx1Str = splitLine(currentLine[1], '/');
      std::vector<std::string> vtx2Str = splitLine(currentLine[2], '/');
      std::vector<std::string> vtx3Str = splitLine(currentLine[3], '/');
      
      processVertex(vtx1Str, indices, textures, normals, mTexturesVector, mNormalsVector);
      processVertex(vtx2Str, indices, textures, normals, mTexturesVector, mNormalsVector);
      processVertex(vtx3Str, indices, textures, normals, mTexturesVector, mNormalsVector);
    }
    
    file.close();
    
    mIndicesVector.resize(indices.size());
    
    for (auto i = 0; i < indices.size(); ++i) {
      mIndicesVector[i] = indices.at(i);
    }

    vertices.clear();
    textures.clear();
    normals.clear();
    indices.clear();
    std::vector<GLuint>{}.swap(indices);
    std::vector<glm::vec3>{}.swap(vertices);
    std::vector<glm::vec2>{}.swap(textures);
    std::vector<glm::vec3>{}.swap(normals);
    
    RawModel model = loader.loadToVAO((GLfloat*) &mVerticesVector[0],
                                      (GLuint*) &mIndicesVector[0],
                                      (GLfloat*) &mTexturesVector[0],
                                      (GLfloat*) &mNormalsVector[0],
                                      (GLuint) mVerticesVector.size(),
                                      (GLuint) mIndicesVector.size(),
                                      (GLuint) mTexturesVector.size(),
                                      (GLuint) mNormalsVector.size());
    
    return model;
  }
  
  void processVertex(const std::vector<std::string>& vtxData, std::vector<GLuint>& indices,
                     std::vector<glm::vec2>& textures, std::vector<glm::vec3>& normals,
                     std::vector<GLfloat>& textureArray, std::vector<GLfloat>& normalsArray) {
    int currentVertexPointer = std::atoi(vtxData[0].c_str()) - 1;
    indices.push_back(currentVertexPointer);
    
    glm::vec2 currentTex = textures.at(std::atoi(vtxData[1].c_str()) - 1);
    textureArray[currentVertexPointer * 2] = currentTex.x;
    textureArray[currentVertexPointer * 2 + 1] = currentTex.y; //1.0 - currentTex.y;
    
    glm::vec3 currentNorm = normals.at(std::atoi(vtxData[2].c_str()) - 1);
    normalsArray[currentVertexPointer * 3] = currentNorm.x;
    normalsArray[currentVertexPointer * 3 + 1] = currentNorm.y;
    normalsArray[currentVertexPointer * 3 + 2] = currentNorm.z;
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

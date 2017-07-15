#include "NormalObjLoader.hpp"
#include "Vertex.hpp"
#include <glm/gtx/transform.hpp>
#include <string>

ModelDataNM NormalObjLoader::loadObj(const std::string& filename) {
  std::ifstream file(filename.c_str());
  
  std::vector<VertexNM> vertices;
  std::vector<GLfloat> verticesFlattened;
  
  std::vector<glm::vec2> textures;
  std::vector<GLfloat> texturesFlattened;
  
  std::vector<glm::vec3> normals;
  std::vector<GLfloat> normalsFlattened;
  
  std::vector<glm::vec3> tangents;
  std::vector<GLfloat> tangentsFlattened;
  
  std::vector<GLuint> indices;
  
  std::string lineBeforeBreak;
  
  while (true) {
    std::string line;
    std::getline(file, line);
    
    std::vector<std::string> currentLine = splitLineBySpace(line);
    if (lineStartsWith(line, "v ")) {
      int index = (int) vertices.size();
      VertexNM vtx{index, parseTriplet(currentLine)};
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
  
  auto v0 = processVertex(vtx1Str, vertices, indices);
  auto v1 = processVertex(vtx2Str, vertices, indices);
  auto v2 = processVertex(vtx3Str, vertices, indices);
  calculateTangents(v0, v1, v2, textures);
  
  for (std::string line; std::getline(file, line); ) {
    if (!lineStartsWith(line, "f ")) {
      continue;
    }
    std::vector<std::string> currentLine = splitLineBySpace(line);
    
    std::vector<std::string> vtx1Str = splitLine(currentLine[1], '/');
    std::vector<std::string> vtx2Str = splitLine(currentLine[2], '/');
    std::vector<std::string> vtx3Str = splitLine(currentLine[3], '/');
    
    auto v0 = processVertex(vtx1Str, vertices, indices);
    auto v1 = processVertex(vtx2Str, vertices, indices);
    auto v2 = processVertex(vtx3Str, vertices, indices);
    
    calculateTangents(v0, v1, v2, textures);
  }
  
  file.close();
  removeUnusedVertices(vertices);
  
  verticesFlattened.resize(vertices.size() * 3);
  texturesFlattened.resize(vertices.size() * 2);
  normalsFlattened.resize(vertices.size() * 3);
  tangentsFlattened.resize(vertices.size() * 3);
  
  for (auto i = 0; i < vertices.size(); ++i) {
    auto vertex = vertices[i];
    auto position = vertex.mPosition;
    auto texCoord = textures[vertex.mTextureIndex];
    auto normal = normals[vertex.mNormalIndex];
    auto tangent = vertex.calculateAverageTangent();
    
    verticesFlattened[i * 3] = position.x;
    verticesFlattened[i * 3 + 1] = position.y;
    verticesFlattened[i * 3 + 2] = position.z;
    
    texturesFlattened[i * 2] = texCoord.x;
    texturesFlattened[i * 2 + 1] = texCoord.y;
    
    normalsFlattened[i * 3] = normal.x;
    normalsFlattened[i * 3 + 1] = normal.y;
    normalsFlattened[i * 3 + 2] = normal.z;
    
    tangentsFlattened[i * 3] = tangent.x;
    tangentsFlattened[i * 3 + 1] = tangent.y;
    tangentsFlattened[i * 3 + 2] = tangent.z;
  }
  
  float furthest = findFurthestPoint(vertices);
  
  return ModelDataNM{verticesFlattened, texturesFlattened, normalsFlattened, tangentsFlattened, indices, furthest};
}

void NormalObjLoader::calculateTangents(VertexNM* v0, VertexNM* v1, VertexNM* v2, const std::vector<glm::vec2>& uvs) {
  glm::vec3 edge0 = v1->mPosition - v0->mPosition;
  glm::vec3 edge1 = v2->mPosition - v0->mPosition;
  
  glm::vec2 uv0 = uvs.at(v0->mTextureIndex);
  glm::vec2 uv1 = uvs.at(v1->mTextureIndex);
  glm::vec2 uv2 = uvs.at(v2->mTextureIndex);
  glm::vec2 deltaUv0 = uv1 - uv0;
  glm::vec2 deltaUv1 = uv2 - uv0;
  
  float r = 1.0f / (deltaUv0.x * deltaUv1.y - deltaUv0.y * deltaUv1.x);
  edge0 *= deltaUv1.y;
  edge1 *= deltaUv0.y;
  glm::vec3 tangent = edge0 - edge1;
  tangent *= r;
  v0->mTangents.push_back(tangent);
  v1->mTangents.push_back(tangent);
  v2->mTangents.push_back(tangent);
}


float NormalObjLoader::findFurthestPoint(const std::vector<VertexNM>& vertices) {
  float furthestPoint = std::numeric_limits<float>::min();
  for (auto v : vertices) {
    if (v.mLength > furthestPoint) furthestPoint = v.mLength;
  }
  return furthestPoint;
}

VertexNM* NormalObjLoader::processVertex(const std::vector<std::string>& vtxData, std::vector<VertexNM>& vertices, std::vector<GLuint>& indices) {
  GLuint currentVertexIndex = (GLuint) std::atoi(vtxData[0].c_str()) - 1;
  GLuint currentTextureIndex = (GLuint) std::atoi(vtxData[1].c_str()) - 1;
  GLuint currentNormalIndex = (GLuint) std::atoi(vtxData[2].c_str()) - 1;
  
  VertexNM* currentVertex = &vertices.at(currentVertexIndex);
  
  if (!currentVertex->isSet()) {
    currentVertex->mTextureIndex = currentTextureIndex;
    currentVertex->mNormalIndex = currentNormalIndex;
    indices.push_back(currentVertexIndex);
    return currentVertex;
  }
  else {
    return handleProcessedVertex(currentVertex, currentTextureIndex, currentNormalIndex, indices, vertices);
  }
}

VertexNM* NormalObjLoader::handleProcessedVertex(VertexNM* previousVertex, GLuint newTextureIndex, GLuint newNormalIndex, std::vector<GLuint>& indices, std::vector<VertexNM>& vertices) {
  if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
    indices.push_back(previousVertex->mIndex);
    return previousVertex;
  }
  else {
    std::shared_ptr<VertexNM> anotherVertex = previousVertex->mDuplicate;
    if (nullptr != anotherVertex) {
      return handleProcessedVertex(anotherVertex.get(), newTextureIndex, newNormalIndex, indices, vertices);
    }
    else {
      std::shared_ptr<VertexNM> duplicateVertex = previousVertex->duplicate(vertices.size());
      
      
      duplicateVertex->mTextureIndex = newTextureIndex;
      duplicateVertex->mNormalIndex = newNormalIndex;
      previousVertex->mDuplicate = duplicateVertex;
      vertices.push_back(*duplicateVertex);
      indices.push_back(duplicateVertex->mIndex);
      return duplicateVertex.get();
    }
  }
}

void NormalObjLoader::removeUnusedVertices(std::vector<VertexNM>& vertices) {
  for (auto v : vertices) {
    if (!v.isSet()) {
      v.mTextureIndex = 0;
      v.mNormalIndex = 0;
    }
  }
}

std::string NormalObjLoader::readObjFile(const std::string& filename) {
  std::ifstream file(filename.c_str());
  
  if (!file.good()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  
  std::stringstream stream;
  stream << file.rdbuf();
  file.close();
  return stream.str();
}

std::vector<std::string> NormalObjLoader::splitLineBySpace(const std::string& line) {
  return splitLine(line, ' ');
}

std::vector<std::string> NormalObjLoader::splitLine(const std::string& line, char delim) {
  std::stringstream ss;
  ss.str(line);
  std::string item;
  std::vector<std::string> strs;
  while (std::getline(ss, item, delim)) {
    strs.push_back(item);
  }
  return strs;
}

bool NormalObjLoader::lineStartsWith(const std::string& line, const std::string& subStr) {
  return 0 == std::strncmp(line.c_str(), subStr.c_str(), std::strlen(subStr.c_str()));
}

glm::vec2 NormalObjLoader::parsePair(const std::vector<std::string>& line) {
  return glm::vec2{std::atof(line[1].c_str()), std::atof(line[2].c_str())};
}

glm::vec3 NormalObjLoader::parseTriplet(const std::vector<std::string>& line) {
  return glm::vec3{std::atof(line[1].c_str()), std::atof(line[2].c_str()), std::atof(line[3].c_str())};
}

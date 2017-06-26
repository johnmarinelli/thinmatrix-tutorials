#ifndef ObjModel_hpp
#define ObjModel_hpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

class ObjModel {
public:
  ObjModel(std::string path) {
    loadModel(path);
  }
  
  void draw(const ShaderProgram& shaderProgram) {
    for (auto mesh : mMeshes) {
      mesh.draw(shaderProgram);
    }
  }
  
  std::vector<Mesh> mMeshes;
  
  void loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
      return;
    }
    
    processNode(scene->mRootNode, scene);
  }
  
  void processNode(aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      mMeshes.push_back(processMesh(mesh, scene));
     }
    
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<ObjTexture> textures;
    
    for (auto i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;
      glm::vec3 v;
      v.x = mesh->mVertices[i].x;
      v.y = mesh->mVertices[i].y;
      v.z = mesh->mVertices[i].z;
      vertex.mPosition = v;
      
      glm::vec3 n;
      n.x = mesh->mNormals[i].x;
      n.y = mesh->mNormals[i].y;
      n.z = mesh->mNormals[i].z;
      vertex.mNormal = n;
      
      glm::vec2 t;
      if (mesh->mTextureCoords[0]) {
        t.x = mesh->mTextureCoords[0][i].x;
        t.y = mesh->mTextureCoords[0][i].y;
      }
      else {
        t.x = 0.0f;
        t.y = 0.0f;
      }
      vertex.mTexCoord = t;
      
      vertices.push_back(vertex);
    }
    
    for (auto i = 0; i < mesh->mNumFaces; ++i) {
      aiFace face = mesh->mFaces[i];
      
      // get all indices of face and store them in indices vector
      for (auto j = 0; j < face.mNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }
    
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    }
    
    return Mesh{vertices, indices, textures};
  }
  
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    return std::vector<Texture>{};
  }
};

#endif

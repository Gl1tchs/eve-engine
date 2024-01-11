// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "asset/asset_registry.h"
#include "core/debug/log.h"
#include "graphics/texture.h"

namespace eve {

static std::vector<Ref<Texture>> loaded_textures{};

static Ref<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                         TextureType tex_type,
                                         fs::path directory);

Ref<Model> Model::Create(const fs::path& path) {
  Ref<Model> model = CreateRef<Model>();

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                         aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    LOG_ENGINE_ERROR("Unable to load model: {}", importer.GetErrorString());
    return nullptr;
  }

  model->directory_ = path.parent_path();

  // process ASSIMP's root node recursively
  model->ProcessNode(scene->mRootNode, scene);

  return model;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  // process each mesh located at the current node
  for (uint32_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(ProcessMesh(mesh, scene));
  }

  for (uint32_t i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

RenderData<MeshVertex> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<MeshVertex> vertices;

  // walk through each of the mesh's vertices
  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    MeshVertex vertex;
    glm::vec4 vector;

    // positions
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vector.w = 1.0f;

    vertex.position = vector;

    // normals
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vector.w = mesh->mNormals[i].z;
      vertex.normal = vector;
    }

    // texture coordinates
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
      // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.tex_coords = vec;

      // TODO
      // // tangent
      // vector.x = mesh->mTangents[i].x;
      // vector.y = mesh->mTangents[i].y;
      // vector.z = mesh->mTangents[i].z;
      // vertex.Tangent = vector;
      // // bitangent
      // vector.x = mesh->mBitangents[i].x;
      // vector.y = mesh->mBitangents[i].y;
      // vector.z = mesh->mBitangents[i].z;
      // vertex.Bitangent = vector;
    } else {
      vertex.tex_coords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  std::vector<uint32_t> indices;

  for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    // retrieve all indices of the face and store them in the indices vector
    for (uint32_t j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
  // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
  // Same applies to other texture as the following list summarizes:
  // diffuse: texture_diffuseN
  // specular: texture_specularN
  // normal: texture_normalN

  RenderData<MeshVertex> render_data;
  render_data.vertices = vertices;
  render_data.indices = indices;

  render_data.diffuse_map = LoadMaterialTextures(
      material, aiTextureType_DIFFUSE, TextureType::kDiffuse, directory_);

  render_data.specular_map = LoadMaterialTextures(
      material, aiTextureType_SPECULAR, TextureType::kSpecular, directory_);

  render_data.normal_map = LoadMaterialTextures(
      material, aiTextureType_HEIGHT, TextureType::kNormal, directory_);

  render_data.height_map = LoadMaterialTextures(
      material, aiTextureType_AMBIENT, TextureType::kHeight, directory_);

  return render_data;
}

Ref<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                  TextureType tex_type, fs::path directory) {
  if (mat->GetTextureCount(type) <= 0) {
    return nullptr;
  }

  // TODO multiple textures
  // for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  // mat->GetTexture(type, i, &str);

  aiString str;
  mat->GetTexture(type, 0, &str);

  fs::path file_path = directory / fs::path(str.C_Str()).filename();

  // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
  const auto it = std::find_if(loaded_textures.begin(), loaded_textures.end(),
                               [file_path](const Ref<Texture>& texture) {
                                 return texture->path == file_path;
                               });

  if (it != loaded_textures.end()) {
    return *it;
  }

  // if texture hasn't been loaded already, load it
  Ref<Texture> texture = Texture::Create(file_path, tex_type);

  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
  loaded_textures.push_back(texture);

  return texture;
}

}  // namespace eve

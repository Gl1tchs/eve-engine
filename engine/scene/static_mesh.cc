// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/static_mesh.h"

#include <OBJ_Loader.h>

#include "core/debug/log.h"

glm::vec2 ToGLM2(const objl::Vector2& from) {
  return glm::vec2{from.X, from.Y};
}

glm::vec3 ToGLM3(const objl::Vector3& from) {
  return glm::vec3{from.X, from.Y, from.Z};
}

Ref<Model> Model::Load(const std::filesystem::path& path) {
  objl::Loader loader;

  bool success = loader.LoadFile(path.string());
  if (!success) {
    LOG_ERROR("Unable to load mesh from path: {0}.", path.string());
    return nullptr;
  }

  Ref<Model> model = CreateRef<Model>();

  for (auto mesh_loaded : loader.LoadedMeshes) {
    RenderData<MeshVertex> mesh;

    mesh.name = mesh_loaded.MeshName;
    for (auto vertex_loaded : mesh_loaded.Vertices) {
      MeshVertex vertex;
      vertex.position = glm::vec4(ToGLM3(vertex_loaded.Position), 1.0);
      vertex.normal = ToGLM3(vertex_loaded.Normal);
      vertex.tex_coords = ToGLM2(vertex_loaded.TextureCoordinate);

      mesh.vertices.push_back(vertex);
    }

    mesh.indices = mesh_loaded.Indices;

    Material material{};
    // material.ambient = ToGLM3(mesh_loaded.MeshMaterial.);
    // material.diffuse = ToGLM3(mesh_loaded.MeshMaterial.Kd);
    // material.specular = ToGLM3(mesh_loaded.MeshMaterial.Ks);
    // material.shininess = mesh_loaded.MeshMaterial.Ns;
    // TODO material textures

    mesh.material = material;

    model->meshes.push_back(mesh);
  }

  return model;
}
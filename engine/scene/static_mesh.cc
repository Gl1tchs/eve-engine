// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/static_mesh.h"

#include <OBJ-Loader/OBJ_Loader.h>

#include "core/debug/log.h"

Ref<Model> Model::Load(const std::filesystem::path& path) {
  objl::Loader loader;

  bool success = loader.LoadFile(path.string());
  if (!success) {
    LOG_ENGINE_ERROR("Unable to load mesh from path: {0}.", path.string());
    return nullptr;
  }

  Ref<Model> model = CreateRef<Model>();

  for (auto mesh_loaded : loader.LoadedMeshes) {
    StaticMesh mesh;

    mesh.name = mesh_loaded.MeshName;
    for (auto vertex_loaded : mesh_loaded.Vertices) {
      Vertex vertex;
      vertex.position = {vertex_loaded.Position.X, vertex_loaded.Position.Y,
                         vertex_loaded.Position.Z, 1.0f};
      vertex.normal = {vertex_loaded.Normal.X, vertex_loaded.Normal.Y,
                       vertex_loaded.Normal.Z};
      vertex.tex_coords = {vertex_loaded.TextureCoordinate.X,
                           vertex_loaded.TextureCoordinate.Y};

      mesh.vertices.push_back(vertex);
    }

    mesh.indices = mesh_loaded.Indices;

    model->meshes.push_back(mesh);
  }

  Material material;
  // TODO import material
  material.color = {0.31f, 0.62f, 0.93f, 1.0f};

  model->material = material;
  
  return model;
}
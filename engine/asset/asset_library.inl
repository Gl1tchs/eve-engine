template <>
inline AssetRef<Texture> AssetLibrary::LoadFromMeta(const std::string& path) {
  return AssetLoader::LoadTexture(path);
}

template <>
inline AssetRef<Model> AssetLibrary::LoadFromMeta(const std::string& path) {
  return AssetLoader::LoadModel(path);
}

namespace eve {

template <>
inline AssetRef<Texture> AssetLibrary::Load(const std::string& path) {
  return AssetLoader::LoadTexture(path);
}

template <>
inline AssetRef<Model> AssetLibrary::Load(const std::string& path) {
  return AssetLoader::LoadModel(path);
}

}  // namespace eve
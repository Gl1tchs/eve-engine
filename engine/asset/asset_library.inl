template <>
inline AssetRef<Texture> AssetLibrary::LoadFromMeta(const std::string& path) {
  AssetRef<Texture> asset = AssetLoader::LoadTexture(path);
  assets_[path] = asset->info;
  return asset;
}

template <>
inline AssetRef<Model> AssetLibrary::LoadFromMeta(const std::string& path) {
  AssetRef<Model> asset = AssetLoader::LoadModel(path);
  assets_[path] = asset->info;
  return asset;
}

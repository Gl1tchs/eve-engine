template <>
inline AssetRef<Texture> AssetLibrary::LoadFromMeta(const std::string& path) {
  AssetRef<Texture> asset = AssetLoader::LoadTexture(path);
  assets_[path] = asset->info;
  return asset;
}

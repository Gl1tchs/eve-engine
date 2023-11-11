template <>
inline AssetRef<Texture> AssetLibrary::LoadFromMeta(
    const std::filesystem::path& path) {
  AssetRef<Texture> asset = AssetLoader::LoadTexture(path);
  assets_[path] = asset->info;
  return asset;
}

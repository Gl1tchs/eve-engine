template <typename T>
inline AssetRef<T> AssetLibrary::LoadFromPath(const std::string& path) {
  std::string meta_path(std::format("{0}.meta", path));
  if (!std::filesystem::exists(GetAssetPath(meta_path))) {
    LOG_ERROR(
        "Unable to load asset from: {0}\nAsset is not imported (.meta not "
        "exists).",
        path);
    return nullptr;
  }

  return LoadFromMeta<T>(meta_path);
}

template <>
inline AssetRef<Texture> AssetLibrary::LoadFromMeta(const std::string& path) {
  return AssetLoader::LoadTexture(path);
}

template <>
inline AssetRef<Model> AssetLibrary::LoadFromMeta(const std::string& path) {
  return AssetLoader::LoadModel(path);
}

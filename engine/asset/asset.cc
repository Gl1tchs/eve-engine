// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset.h"
#include "asset.h"

namespace eve {

AssetType GetAssetTypeFromExtension(const std::string& extension) {
  if (extension == ".cs") {
    return AssetType::kScript;
  } else if (extension == ".fbx" || extension == ".dae" ||
             extension == ".gltf" || extension == ".glb" ||
             extension == ".blend" || extension == ".3ds" ||
             extension == ".ase" || extension == ".obj") {
    return AssetType::kStaticMesh;
  } else if (extension == ".mp3" || extension == ".ogg" ||
             extension == ".wav" || extension == ".aac") {
    return AssetType::kAudio;
  } else if (extension == ".png" || extension == ".jpg" ||
             extension == ".jpeg" || extension == ".webp") {
    return AssetType::kTexture;
  } else if (extension == ".glsl" || extension == ".esh") {
    return AssetType::kShader;
  } else if (extension == ".ttf") {
    return AssetType::kFont;
  } else {
    return AssetType::kNone;
  }
}

std::string GetAssetTypeString(AssetType type) {

  switch (type) {
    case AssetType::kNone:
      return "NONE";
    case AssetType::kTexture:
      return "TEXTURE";
    case AssetType::kFont:
      return "FONT";
    case AssetType::kScene:
      return "SCENE";
    case AssetType::kStaticMesh:
      return "MESH";
    case AssetType::kScript:
      return "SCRIPT";
    case AssetType::kMaterial:
      return "MATERIAL";
    case AssetType::kShader:
      return "SHADER";
    case AssetType::kAudio:
      return "AUDIO";
    default:
      return "";
  }
}

}  // namespace eve

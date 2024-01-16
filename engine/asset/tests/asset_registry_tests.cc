#include "catch2/catch_all.hpp"

#include "asset/asset_registry.h"
#include "project/project.h"

using namespace eve;

struct DummyAsset : Asset {
  EVE_IMPL_ASSET(AssetType::kNone);

  DummyAsset(std::string&& name, const AssetHandle& handle) {
    this->name = name;
    this->handle = handle;
  }
};

void CreateDummyProject() {
  Ref<Project> project = CreateRef<Project>(".");
  ProjectConfig& config = project->GetConfig();
  config.asset_directory = "assets";

  Project::SetActive(project);
}

TEST_CASE("Asset Registration and Retrieval", "[AssetRegistry]") {
  CreateDummyProject();

  AssetHandle dummy_handle = 1;
  Ref<DummyAsset> dummy_asset =
      CreateRef<DummyAsset>("DummyAsset", dummy_handle);

  AssetRegistry::Register(dummy_asset);

  Ref<DummyAsset> retrieved_asset =
      AssetRegistry::Get<DummyAsset>(dummy_handle);

  REQUIRE(retrieved_asset == dummy_asset);
}

TEST_CASE("Test Path Utils", "[AssetRegistry]") {
  CreateDummyProject();

  SECTION("Absolute Path Retrivation") {
    const std::string path_relative = "res://textures/my_texture.png";
    const fs::path path_absolute = "assets/textures/my_texture.png";

    REQUIRE(AssetRegistry::GetAssetPath(path_relative) == path_absolute);
  }

  SECTION("Relative Path Retrivation") {
    const std::string path_absolute = "assets/textures/my_texture.png";
    const std::string path_relative = "res://textures/my_texture.png";

    REQUIRE(fs::path(AssetRegistry::GetRelativePath(path_absolute)) ==
            fs::path(path_relative));
  }

  SECTION("Trimmed Path Retrivation") {
    const std::string path_untrimmed = "res://textures/my_texture.png";
    const std::string path_trimmed = "textures/my_texture.png";

    REQUIRE(AssetRegistry::GetAssetPathTrimmed(path_untrimmed) == path_trimmed);
  }
}
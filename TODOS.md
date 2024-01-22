# EVE Todo

## General
- [ ] Continius integration and testing.
- [ ] Register eve_editor to windows in order to open .eproject files directly.
- [ ] In Game UI
- [ ] Make mono work on linux.
- [ ] Sounds
- [ ] Engine plugins C#/C++

## Editor
- [ ] Texture gizmos for lights and camera.
- [ ] Scene settings
  - [ ] Skybox
  - [x] Editor Camera
- [ ] Import asset from editor
    - [ ] Configure assets from editor like texture repeat mode and such.
- [ ] CTRL+C CTRL+V copy paste entities
- [ ] New Project
- [ ] Save editor state with cache and temp files.
- [ ] While scripts are building show a loading thing for it.

## Asset
- [ ] Remove requirement of asset registry window on editor.
- [ ] Assets should be scene only and only the ones on the scene should be loaded.
- [ ] Make physics settings an asset.
- [ ] Do not load unusued assets to memory.
- [ ] Better asset system with proper registry and metadata caching.
- [ ] Asset compression for shipping.

## Scene
- [ ] Entity prefabs.
- [ ] When meshes are loaded create submeshes of that instance.
- [ ] Hot reload scene.
- [ ] Make EnTT library private.
- [ ] Add new scenes to the build on create.

## Physics
- [ ] Collision layers.
- [ ] Capsule and mesh colliders.
- [ ] Forces and impulses.
- [ ] Angular velocity and torque.

## Scripting
- [ ] Default script fields not working.
- [ ] Get children from scripts.
- [ ] Asset system scripting API.
- [ ] Texture and Mesh data structures.
- [ ] Build project in release mode when shipping.
- [ ] Current way to compile script project is not good.
- [ ] Script lifetime multiple scenes (kinda like unity's DontDestroyOnLoad)
- [ ] Add mat4 data type
- [ ] API Documentation / code + markdown

## Renderer
- [ ] Add render passes.
- [ ] GPU Particle System
- [ ] Multiple texture support (multiple diffusion, specular, normal, height).
- [ ] HDR Environment maps.
- [ ] Custom shaders with scripting API.
    - [x] Custom shaders.
    - [ ] Scripting api (requires proper asset system).
- [ ] Text rendering.
- [ ] PBR.
- [ ] Other light sources.

## Shaders

## Bugs
- [ ] Make mesh renderer to use batches in order to render millions of indices.
- [ ] Scenes that are not in project file couldn't open.

## Done
- [x] Add engine api to scripting
- [x] Script editor interface
- [x] Script serialize fields
- [x] If script doesnt exist dont let user to start.
- [x] Register entity type.
- [x] Add an external scene renderer class.
- [x] Scene management API
- [x] Start the script if added during runtime.
- [x] Hot reload script
- [x] File/Open Scene not working.
- [x] An runtime application
- [x] Improve engine api for scripting.
- [x] Use a custom format rather yaml.
- [x] Add content browser and use id based asset system.
    - When using an asset inside of the inspector it should keep the id and find that asset in asset manager
    but should display the name of it based on the asset metadata.
- [x] Project settings
  - [x] Asset registry
  - [x] Name, AssetDirectory, Scenes
- [x] Documentate and make shader variables more proper. For example (SCREEN_TEXTURE, VERTEX_POS, WORLD_POS, NORMAL_MATRIX and such.)
- [x] Texture rendering in detail (normal maps, height maps, materials).
- [x] Add basic shapes to library (cube, plane, pyramid, sphere etc.).
- [x] Physics
- [x] Stepping not working
- [x] Better toolbar.
- [x] Add trigger functionality to C#.
- [x] Create entities from script (Instantiate<EntityClass> -> EntityClass | Instantiate() -> ScriptEntity)
- [x] Add parent / child entity system.
- [x] Static application and Editor class to handle quit events.
- [x] Add an transform system to handle parent/child transforms.
- [x] Project opener / creater application
  - Create template scripts
- [x] DragFloat3 label's not showing into editor fields.
- [x] Inspector script entity field

## Fixed
- [x] Cannot switch between projects.
- [x] Editor closes very slowly
- [x] There is a problem with sprites I cannot render more then one and
sometimes it shown half.

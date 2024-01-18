# EVE Todo

## General
- [ ] Continius integration and testing.
- [ ] Register eve_editor to windows in order to open .eproject files directly. 
- [ ] Project opener / creater application
    - Create template scripts
- [ ] In Game UI
- [ ] Sounds

## Editor
- [ ] DragFloat3 label's not showing into editor fields.
- [ ] Texture gizmos for lights and camera.
- [ ] Scene settings
  - [ ] Skybox
  - [x] Editor Camera
- [ ] Import asset from editor
    - [ ] Configure assets from editor like texture repeat mode and such.
- [ ] CTRL+C CTRL+V copy paste entities
- [ ] New Project
- [ ] Inspector script entity field
  - [ ] Prefabs
  - [ ] Use ImGui::DragAndDropField
- [ ] Save editor state with cache and temp files.
- [ ] While scripts are building show a loading thing for it.

## Asset
- [ ] Do not load unusued assets to memory.
- [ ] Better asset system with proper registry and metadata caching.
- [ ] Asset compression for shipping.

## Scene
- [ ] Add an transform system to handle parent/child transforms.
- [ ] When meshes are loaded create submeshes of that instance.
- [ ] Hot reload scene.
- [ ] Make EnTT library private.

## Physics
- [ ] Collision layers.
- [ ] Capsule and mesh colliders.
- [ ] Forces and impulses.
- [ ] Angular velocity and torque.

## Scripting
- [ ] Asset system scripting API.
- [ ] Build project in release mode when shipping.
- [ ] Add new scenes to the build on create. 
- [ ] Current way to compile script project is not good.
- [ ] Script lifetime multiple scenes (kinda like unity's DontDestroyOnLoad)
- [ ] Add mat4 data type
- [ ] API Documentation / code + markdown

## Renderer
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

## Fixed
- [x] Cannot switch between projects.
- [x] Editor closes very slowly

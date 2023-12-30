# EVE Todo

## General
- [ ] Register eve_editor to windows in order to open .eproject files directly. 
- [ ] Project opener / creater application
    - Create template scripts
- [ ] In Game UI
- [ ] Physics
- [ ] Use a custom format rather yaml.
- [ ] Sounds

## Editor
- [ ] New Project
- [ ] Inspector script entity field
- [ ] Better toolbar.
- [ ] Add content browser and use id based asset system.
    - When using an asset inside of the inspector it should keep the id and find that asset in asset manager
    but should display the name of it based on the asset metadata.
- [ ] Save editor state with cache and temp files.
- [ ] While scripts are building show a loading thing for it.

## Asset
- [ ] Asset compression for shipping.

## Scene
- [ ] Hot reload scene.
- [ ] Make EnTT library private.

## Scripting
- [ ] Add new scenes to the build on create. 
- [ ] Script lifetime multiple scenes (kinda like unity's DontDestroyOnLoad)
- [ ] Add mat4 data type
- [ ] Create entities from script (Instantiate<EntityClass> -> EntityClass | Instantiate() -> ScriptEntity)
- [ ] API Documentation / code + markdown

## Renderer
- [ ] Text rendering
- [ ] Add basic shapes to library (cube, plane, pyramid, sphere etc.)
- [ ] Texture rendering in detail (normal maps, height maps, materials).
- [ ] PBR.
- [ ] Other light sources
- [ ] Custom shaders with scripting API.

## Bugs
- [ ] Cannot switch between projects.
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

## Fixed
- [x] Editor closes very slowly

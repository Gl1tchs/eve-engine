function OnStart() 
    local camera_comp_ = AddComponent_CameraComponent();
    camera_comp_.is_primary = true;

    local persp_camera = camera_comp_.perspective_camera;
    persp_camera.fov = 45;
    persp_camera.near_clip = 0.1;
    persp_camera.far_clip = 300.0;
end

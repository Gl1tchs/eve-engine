namespace EveEngine
{
  /// <summary>
  /// Abstract class representing a camera on scene.
  /// </summary>
  public abstract class Camera
  {
    /// <summary>
    /// Aspect ratio of the screen.
    /// </summary>
    public float AspectRatio;

    /// <summary>
    /// Entity object which is attached to the camera.
    /// </summary>
    protected internal Entity Entity;

    internal void SetEntity(Entity entity)
    {
      Entity = entity;
    }
  }

  /// <summary>
  /// Class representing a orthographic camera on scene.
  /// </summary>
  public class OrthographicCamera : Camera
  {
    /// <summary>
    /// Camera frustum box size.
    /// </summary>
    public float ZoomLevel
    {
      get
      {
        Interop.CameraComponent_OrthographicCamera_GetAspectRatio(Entity.Id, out float aspectRatio);
        return aspectRatio;
      }
      set
      {
        Interop.CameraComponent_OrthographicCamera_SetAspectRatio(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Camera near clip. Objects will appear after this value.
    /// </summary>
    public float NearClip
    {
      get
      {
        Interop.CameraComponent_OrthographicCamera_GetNearClip(Entity.Id, out float nearClip);
        return nearClip;
      }
      set
      {
        Interop.CameraComponent_OrthographicCamera_SetNearClip(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Camera far clip. Objects will stop appering after this value.
    /// </summary>
    public float FarClip
    {
      get
      {
        Interop.CameraComponent_OrthographicCamera_GetFarClip(Entity.Id, out float farClip);
        return farClip;
      }
      set
      {
        Interop.CameraComponent_OrthographicCamera_SetFarClip(Entity.Id, ref value);
      }
    }
  }

  /// <summary>
  /// Class representing a perspective camera on scene.
  /// </summary>
  public class PerspectiveCamera : Camera
  {
    /// <summary>
    /// 3D Camera frustum vertical field of view.
    /// </summary>
    public float Fov
    {
      get
      {
        Interop.CameraComponent_PerspectiveCamera_GetFov(Entity.Id, out float fov);
        return fov;
      }
      set
      {
        Interop.CameraComponent_PerspectiveCamera_SetFov(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Camera near clip. Objects will appear after this value.
    /// </summary>
    public float NearClip
    {
      get
      {
        Interop.CameraComponent_PerspectiveCamera_GetNearClip(Entity.Id, out float nearClip);
        return nearClip;
      }
      set
      {
        Interop.CameraComponent_PerspectiveCamera_SetNearClip(Entity.Id, ref value);
      }
    }


    /// <summary>
    /// Camera far clip. Objects will stop appering after this value.
    /// </summary>
    public float FarClip
    {
      get
      {
        Interop.CameraComponent_PerspectiveCamera_GetFarClip(Entity.Id, out float farClip);
        return farClip;
      }
      set
      {
        Interop.CameraComponent_PerspectiveCamera_SetFarClip(Entity.Id, ref value);
      }
    }
  }
}
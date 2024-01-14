namespace EveEngine
{
  public abstract class Camera
  {
    public float AspectRatio;

    protected Entity Entity;

    public void SetEntity(Entity entity)
    {
      Entity = entity;
    }
  }

  public class OrthographicCamera : Camera
  {

    public float ZoomLevel
    {
      get
      {
        InternalCalls.CameraComponent_OrthographicCamera_GetAspectRatio(Entity.Id, out float aspectRatio);
        return aspectRatio;
      }
      set
      {
        InternalCalls.CameraComponent_OrthographicCamera_SetAspectRatio(Entity.Id, ref value);
      }
    }
    public float NearClip
    {
      get
      {
        InternalCalls.CameraComponent_OrthographicCamera_GetNearClip(Entity.Id, out float nearClip);
        return nearClip;
      }
      set
      {
        InternalCalls.CameraComponent_OrthographicCamera_SetNearClip(Entity.Id, ref value);
      }
    }
    public float FarClip
    {
      get
      {
        InternalCalls.CameraComponent_OrthographicCamera_GetFarClip(Entity.Id, out float farClip);
        return farClip;
      }
      set
      {
        InternalCalls.CameraComponent_OrthographicCamera_SetFarClip(Entity.Id, ref value);
      }
    }
  }

  public class PerspectiveCamera : Camera
  {
    public float Fov
    {
      get
      {
        InternalCalls.CameraComponent_PerspectiveCamera_GetFov(Entity.Id, out float fov);
        return fov;
      }
      set
      {
        InternalCalls.CameraComponent_PerspectiveCamera_SetFov(Entity.Id, ref value);
      }
    }
    public float NearClip
    {
      get
      {
        InternalCalls.CameraComponent_PerspectiveCamera_GetNearClip(Entity.Id, out float nearClip);
        return nearClip;
      }
      set
      {
        InternalCalls.CameraComponent_PerspectiveCamera_SetNearClip(Entity.Id, ref value);
      }
    }
    public float FarClip
    {
      get
      {
        InternalCalls.CameraComponent_PerspectiveCamera_GetFarClip(Entity.Id, out float farClip);
        return farClip;
      }
      set
      {
        InternalCalls.CameraComponent_PerspectiveCamera_SetFarClip(Entity.Id, ref value);
      }
    }
  }
}
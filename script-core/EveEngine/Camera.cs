namespace EveEngine
{
  public abstract class Camera
  {
    public float aspectRatio;

    protected ScriptEntity entity;

    public void SetEntity(ScriptEntity entity)
    {
      this.entity = entity;
    }
  }

  public class OrthographicCamera : Camera
  {

    public float ZoomLevel
    {
      get
      {
        InternalCalls.CameraComponent_OrthographicCamera_GetAspectRatio(entity.Id, out float aspectRatio);
        return aspectRatio;
      }
      set
      {
        InternalCalls.CameraComponent_OrthographicCamera_SetAspectRatio(entity.Id, ref value);
      }
    }
    public float NearClip
    {
      get
      {
        InternalCalls.CameraComponent_OrthographicCamera_GetNearClip(entity.Id, out float nearClip);
        return nearClip;
      }
      set
      {
        InternalCalls.CameraComponent_OrthographicCamera_SetNearClip(entity.Id, ref value);
      }
    }
    public float FarClip
    {
      get
      {
        InternalCalls.CameraComponent_OrthographicCamera_GetFarClip(entity.Id, out float farClip);
        return farClip;
      }
      set
      {
        InternalCalls.CameraComponent_OrthographicCamera_SetFarClip(entity.Id, ref value);
      }
    }
  }

  public class PerspectiveCamera : Camera
  {
    public float Fov
    {
      get
      {
        InternalCalls.CameraComponent_PerspectiveCamera_GetFov(entity.Id, out float fov);
        return fov;
      }
      set
      {
        InternalCalls.CameraComponent_PerspectiveCamera_SetFov(entity.Id, ref value);
      }
    }
    public float NearClip
    {
      get
      {
        InternalCalls.CameraComponent_PerspectiveCamera_GetNearClip(entity.Id, out float nearClip);
        return nearClip;
      }
      set
      {
        InternalCalls.CameraComponent_PerspectiveCamera_SetNearClip(entity.Id, ref value);
      }
    }
    public float FarClip
    {
      get
      {
        InternalCalls.CameraComponent_PerspectiveCamera_GetFarClip(entity.Id, out float farClip);
        return farClip;
      }
      set
      {
        InternalCalls.CameraComponent_PerspectiveCamera_SetFarClip(entity.Id, ref value);
      }
    }
  }
}
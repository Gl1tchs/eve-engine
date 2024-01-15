namespace EveEngine
{
  public class CameraComponent : Component
  {
    public OrthographicCamera OrthoCamera
    {
      get
      {
        OrthoCamera.SetEntity(Entity);
        return OrthoCamera;
      }
    }
    public PerspectiveCamera PerspCamera
    {
      get
      {
        PerspCamera.SetEntity(Entity);
        return PerspCamera;
      }
    }
    public bool IsOrthographic
    {
      get
      {
        Interop.CameraComponent_GetIsOrthographic(Entity.Id, out bool isOrthographic);
        return isOrthographic;
      }
      set
      {
        Interop.CameraComponent_SetIsOrthographic(Entity.Id, ref value);
      }
    }
    public bool IsPrimary
    {
      get
      {
        Interop.CameraComponent_GetIsPrimary(Entity.Id, out bool isPrimary);
        return isPrimary;
      }
      set
      {
        Interop.CameraComponent_SetIsPrimary(Entity.Id, ref value);
      }
    }
    public bool IsFixedAspectRatio
    {
      get
      {
        Interop.CameraComponent_GetIsFixedAspectRatio(Entity.Id, out bool isFixedAspectRatio);
        return isFixedAspectRatio;
      }
      set
      {
        Interop.CameraComponent_SetIsFixedAspectRatio(Entity.Id, ref value);
      }
    }
  }
}
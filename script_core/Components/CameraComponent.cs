namespace EveEngine
{
  /// <summary>
  /// Represents a camera component attached to an entity.
  /// </summary>
  public class CameraComponent : Component
  {
    /// <summary>
    /// Gets the orthographic camera associated with this camera component.
    /// </summary>
    public OrthographicCamera OrthoCamera
    {
      get
      {
        OrthoCamera.SetEntity(Entity);
        return OrthoCamera;
      }
    }

    /// <summary>
    /// Gets the perspective camera associated with this camera component.
    /// </summary>
    public PerspectiveCamera PerspCamera
    {
      get
      {
        PerspCamera.SetEntity(Entity);
        return PerspCamera;
      }
    }

    /// <summary>
    /// Gets or sets a value indicating whether the camera is in orthographic mode.
    /// </summary>
    public bool IsOrthographic
    {
      get
      {
        Interop.CameraComponent_GetIsOrthographic(Entity.Id, out bool isOrthographic);
        return isOrthographic;
      }
      set => Interop.CameraComponent_SetIsOrthographic(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets or sets a value indicating whether the camera is the primary camera in the scene.
    /// </summary>
    public bool IsPrimary
    {
      get
      {
        Interop.CameraComponent_GetIsPrimary(Entity.Id, out bool isPrimary);
        return isPrimary;
      }
      set => Interop.CameraComponent_SetIsPrimary(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets or sets a value indicating whether the camera has a fixed aspect ratio.
    /// </summary>
    public bool IsFixedAspectRatio
    {
      get
      {
        Interop.CameraComponent_GetIsFixedAspectRatio(Entity.Id, out bool isFixedAspectRatio);
        return isFixedAspectRatio;
      }
      set => Interop.CameraComponent_SetIsFixedAspectRatio(Entity.Id, ref value);
    }
  }
}

namespace EveEngine
{
  /// <summary>
  /// Represents a transform component attached to an entity.
  /// </summary>
  public class Transform : Component
  {
    /// <summary>
    /// Gets or sets the position of the transform.
    /// </summary>
    public Vector3 Position
    {
      get
      {
        Interop.TransformComponent_GetPosition(Entity.Id, out Vector3 position);
        return position;
      }
      set => Interop.TransformComponent_SetPosition(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets or sets the rotation of the transform.
    /// </summary>
    public Vector3 Rotation
    {
      get
      {
        Interop.TransformComponent_GetRotation(Entity.Id, out Vector3 rotation);
        return rotation;
      }
      set => Interop.TransformComponent_SetRotation(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets or sets the scale of the transform.
    /// </summary>
    public Vector3 Scale
    {
      get
      {
        Interop.TransformComponent_GetScale(Entity.Id, out Vector3 scale);
        return scale;
      }
      set => Interop.TransformComponent_SetScale(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets the forward direction of the transform.
    /// </summary>
    /// <returns>The forward vector.</returns>
    public Vector3 GetForward()
    {
      Interop.TransformComponent_GetForward(Entity.Id, out Vector3 forward);
      return forward;
    }

    /// <summary>
    /// Gets the right direction of the transform.
    /// </summary>
    /// <returns>The right vector.</returns>
    public Vector3 GetRight()
    {
      Interop.TransformComponent_GetRight(Entity.Id, out Vector3 right);
      return right;
    }

    /// <summary>
    /// Gets the up direction of the transform.
    /// </summary>
    /// <returns>The up vector.</returns>
    public Vector3 GetUp()
    {
      Interop.TransformComponent_GetUp(Entity.Id, out Vector3 up);
      return up;
    }
  }
}

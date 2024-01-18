namespace EveEngine
{
  /// <summary>
  /// Represents a transform component attached to an entity.
  /// </summary>
  public class Transform : Component
  {
    /// <summary>
    /// Parent transform of current's
    /// </summary>
    public Transform Parent
    {
      get
      {
        if (Entity.Parent is null)
        {
          return null;
        }

        return Entity.Parent.GetComponent<Transform>();
      }
    }

    /// <summary>
    /// Gets or sets the position of the transform.
    /// </summary>
    public Vector3 Position
    {
      get
      {
        Interop.TransformComponent_GetLocalPosition(Entity.Id, out Vector3 position);
        return position;
      }
      set => Interop.TransformComponent_SetLocalPosition(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets or sets the rotation of the transform.
    /// </summary>
    public Vector3 Rotation
    {
      get
      {
        Interop.TransformComponent_GetLocalRotation(Entity.Id, out Vector3 rotation);
        return rotation;
      }
      set => Interop.TransformComponent_SetLocalRotation(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets or sets the scale of the transform.
    /// </summary>
    public Vector3 Scale
    {
      get
      {
        Interop.TransformComponent_GetLocalScale(Entity.Id, out Vector3 scale);
        return scale;
      }
      set => Interop.TransformComponent_SetLocalScale(Entity.Id, ref value);
    }

    /// <summary>
    /// Gets the world position of the transform.
    /// </summary>
    public Vector3 WorldPosition
    {
      get
      {
        Interop.TransformComponent_GetPosition(Entity.Id, out Vector3 position);
        return position;
      }
    }

    /// <summary>
    ///  Gets the world rotation of the transform.
    /// </summary>
    public Vector3 WorldRotation
    {
      get
      {
        Interop.TransformComponent_GetRotation(Entity.Id, out Vector3 rotation);
        return rotation;
      }
    }

    /// <summary>
    ///  Gets the world scale of the transform.
    /// </summary>
    public Vector3 WorldScale
    {
      get
      {
        Interop.TransformComponent_GetScale(Entity.Id, out Vector3 scale);
        return scale;
      }
    }


    /// <summary>
    /// Gets the forward direction of the transform.
    /// </summary>
    /// <returns>The forward vector.</returns>
    public Vector3 Forward
    {
      get
      {
        Interop.TransformComponent_GetForward(Entity.Id, out Vector3 forward);
        return forward;
      }
    }

    /// <summary>
    /// Gets the right direction of the transform.
    /// </summary>
    /// <returns>The right vector.</returns>
    public Vector3 Right
    {
      get
      {
        Interop.TransformComponent_GetRight(Entity.Id, out Vector3 right);
        return right;
      }
    }

    /// <summary>
    /// Gets the up direction of the transform.
    /// </summary>
    /// <returns>The up vector.</returns>
    public Vector3 Up
    {
      get
      {
        Interop.TransformComponent_GetUp(Entity.Id, out Vector3 up);
        return up;
      }
    }

    /// <summary>
    /// Translates the transform by translation.
    /// </summary>
    /// <param name="translation">Value to translate for.</param>
    public void Translate(Vector3 translation)
    {
      Interop.TransformComponent_Translate(Entity.Id, ref translation);
    }

    /// <summary>
    /// Rotates the transform by angle at <c>axis</c>.
    /// </summary>
    /// <param name="angle">Angle to rotate for.</param>
    /// <param name="axis">Axis to rotate on.</param>
    public void Rotate(float angle, Vector3 axis)
    {
      Interop.TransformComponent_Rotate(Entity.Id, angle, ref axis);
    }

    /// <summary>
    /// Makes transform look at the target by rotating.
    /// </summary>
    /// <param name="target">Target to look at.</param>
    public void LookAt(Vector3 target)
    {
      Interop.TransformComponent_LookAt(Entity.Id, ref target);
    }
  }
}

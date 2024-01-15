namespace EveEngine
{
  /// <summary>
  /// Represents constraints for the position of a rigidbody.
  /// </summary>
  public struct RigidbodyPositionConstraints
  {
    /// <summary>
    /// Does physics affect X position on space.
    /// </summary>
    public bool freezeX;

    /// <summary>
    /// Does physics affect Y position on space.
    /// </summary>
    public bool freezeY;

    /// <summary>
    /// Does physics affect Z position on space.
    /// </summary>
    public bool freezeZ;
  }

  /// <summary>
  /// Represents constraints for the rotation of a rigidbody.
  /// </summary>
  public struct RigidbodyRotationConstraints
  {
    /// <summary>
    /// Does physics affect itch rotation on space.
    /// </summary>
    public bool freezePitch;

    /// <summary>
    /// Does physics affect yaw rotation on space.
    /// </summary>
    public bool freezeYaw;

    /// <summary>
    /// Does physics affect roll rotation on space.
    /// </summary>
    public bool freezeRoll;
  }

  /// <summary>
  /// Represents a rigidbody component attached to an entity.
  /// </summary>
  public class Rigidbody : Component
  {
    /// <summary>
    /// Gets or sets the velocity of the rigidbody.
    /// </summary>
    public Vector3 Velocity
    {
      get
      {
        Interop.Rigidbody_GetVelocity(Entity.Id, out Vector3 velocity);
        return velocity;
      }
      set
      {
        Interop.Rigidbody_SetVelocity(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Gets or sets the acceleration of the rigidbody.
    /// </summary>
    public Vector3 Acceleration
    {
      get
      {
        Interop.Rigidbody_GetAcceleration(Entity.Id, out Vector3 acceleration);
        return acceleration;
      }
      set
      {
        Interop.Rigidbody_SetAcceleration(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Gets or sets the mass of the rigidbody.
    /// </summary>
    public float Mass
    {
      get
      {
        Interop.Rigidbody_GetMass(Entity.Id, out float mass);
        return mass;
      }
      set
      {
        Interop.Rigidbody_SetMass(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Gets or sets a value indicating whether gravity affects the rigidbody.
    /// </summary>
    public bool UseGravity
    {
      get
      {
        Interop.Rigidbody_GetUseGravity(Entity.Id, out bool useGravity);
        return useGravity;
      }
      set
      {
        Interop.Rigidbody_SetUseGravity(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Gets or sets the position constraints for the rigidbody.
    /// </summary>
    public RigidbodyPositionConstraints PositionConstraints
    {
      get
      {
        Interop.Rigidbody_GetPositionConstraints(Entity.Id, out RigidbodyPositionConstraints positionConstraints);
        return positionConstraints;
      }
      set
      {
        Interop.Rigidbody_SetPositionConstraints(Entity.Id, ref value);
      }
    }

    /// <summary>
    /// Gets or sets the rotation constraints for the rigidbody.
    /// </summary>
    public RigidbodyRotationConstraints RotationConstraints
    {
      get
      {
        Interop.Rigidbody_GetRotationConstraints(Entity.Id, out RigidbodyRotationConstraints rotationConstraints);
        return rotationConstraints;
      }
      set
      {
        Interop.Rigidbody_SetRotationConstraints(Entity.Id, ref value);
      }
    }
  };
}

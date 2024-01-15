namespace EveEngine
{
  public struct RigidbodyPositionConstraints
  {
    public bool freezeX;
    public bool freezeY;
    public bool freezeZ;
  }

  public struct RigidbodyRotationConstraints
  {
    public bool freezePitch;
    public bool freezeYaw;
    public bool freezeRoll;
  }

  public class Rigidbody : Component
  {
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
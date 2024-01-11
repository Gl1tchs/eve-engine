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
        InternalCalls.Rigidbody_GetVelocity(Entity.Id, out Vector3 velocity);
        return velocity;
      }
      set
      {
        InternalCalls.Rigidbody_SetVelocity(Entity.Id, ref value);
      }
    }

    public Vector3 Acceleration
    {
      get
      {
        InternalCalls.Rigidbody_GetAcceleration(Entity.Id, out Vector3 acceleration);
        return acceleration;
      }
      set
      {
        InternalCalls.Rigidbody_SetAcceleration(Entity.Id, ref value);
      }
    }

    public float Mass
    {
      get
      {
        InternalCalls.Rigidbody_GetMass(Entity.Id, out float mass);
        return mass;
      }
      set
      {
        InternalCalls.Rigidbody_SetMass(Entity.Id, ref value);
      }
    }

    public bool UseGravity
    {
      get
      {
        InternalCalls.Rigidbody_GetUseGravity(Entity.Id, out bool useGravity);
        return useGravity;
      }
      set
      {
        InternalCalls.Rigidbody_SetUseGravity(Entity.Id, ref value);
      }
    }

    public RigidbodyPositionConstraints PositionConstraints
    {
      get
      {
        InternalCalls.Rigidbody_GetPositionConstraints(Entity.Id, out RigidbodyPositionConstraints positionConstraints);
        return positionConstraints;
      }
      set
      {
        InternalCalls.Rigidbody_SetPositionConstraints(Entity.Id, ref value);
      }
    }

    public RigidbodyRotationConstraints RotationConstraints
    {
      get
      {
        InternalCalls.Rigidbody_GetRotationConstraints(Entity.Id, out RigidbodyRotationConstraints rotationConstraints);
        return rotationConstraints;
      }
      set
      {
        InternalCalls.Rigidbody_SetRotationConstraints(Entity.Id, ref value);
      }
    }
  };

}
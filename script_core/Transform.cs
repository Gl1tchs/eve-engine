namespace EveEngine
{
  public class Transform : Component
  {
    public Vector3 Position
    {
      get
      {
        InternalCalls.TransformComponent_GetPosition(Entity.Id, out Vector3 position);
        return position;
      }
      set
      {
        InternalCalls.TransformComponent_SetPosition(Entity.Id, ref value);
      }
    }

    public Vector3 Rotation
    {
      get
      {
        InternalCalls.TransformComponent_GetRotation(Entity.Id, out Vector3 rotation);
        return rotation;
      }
      set
      {
        InternalCalls.TransformComponent_SetRotation(Entity.Id, ref value);
      }
    }

    public Vector3 Scale
    {
      get
      {
        InternalCalls.TransformComponent_GetScale(Entity.Id, out Vector3 scale);
        return scale;
      }
      set
      {
        InternalCalls.TransformComponent_SetScale(Entity.Id, ref value);
      }
    }

    public Vector3 GetForward()
    {
      InternalCalls.TransformComponent_GetForward(Entity.Id, out Vector3 forward);
      return forward;
    }

    public Vector3 GetRight()
    {
      InternalCalls.TransformComponent_GetRight(Entity.Id, out Vector3 right);
      return right;
    }

    public Vector3 GetUp()
    {
      InternalCalls.TransformComponent_GetUp(Entity.Id, out Vector3 up);
      return up;
    }
  }
}
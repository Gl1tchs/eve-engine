namespace EveEngine
{
  public class Transform : Component
  {
    public Vector3 position
    {
      get
      {
        InternalCalls.TransformComponent_GetPosition(entity.id, out Vector3 position);
        return position;
      }
      set
      {
        InternalCalls.TransformComponent_SetPosition(entity.id, ref value);
      }
    }

    public Vector3 rotation
    {
      get
      {
        InternalCalls.TransformComponent_GetRotation(entity.id, out Vector3 rotation);
        return rotation;
      }
      set
      {
        InternalCalls.TransformComponent_SetRotation(entity.id, ref value);
      }
    }

    public Vector3 scale
    {
      get
      {
        InternalCalls.TransformComponent_GetScale(entity.id, out Vector3 scale);
        return scale;
      }
      set
      {
        InternalCalls.TransformComponent_SetScale(entity.id, ref value);
      }
    }

    public Vector3 GetForward()
    {
      Vector3 forward;
      InternalCalls.TransformComponent_GetForward(entity.id, out forward);
      return forward;
    }

    public Vector3 GetRight()
    {
      Vector3 right;
      InternalCalls.TransformComponent_GetRight(entity.id, out right);
      return right;
    }

    public Vector3 GetUp()
    {
      Vector3 up;
      InternalCalls.TransformComponent_GetUp(entity.id, out up);
      return up;
    }
  }
}
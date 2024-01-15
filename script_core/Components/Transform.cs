namespace EveEngine
{
  public class Transform : Component
  {
    public Vector3 Position
    {
      get
      {
        Interop.TransformComponent_GetPosition(Entity.Id, out Vector3 position);
        return position;
      }
      set
      {
        Interop.TransformComponent_SetPosition(Entity.Id, ref value);
      }
    }

    public Vector3 Rotation
    {
      get
      {
        Interop.TransformComponent_GetRotation(Entity.Id, out Vector3 rotation);
        return rotation;
      }
      set
      {
        Interop.TransformComponent_SetRotation(Entity.Id, ref value);
      }
    }

    public Vector3 Scale
    {
      get
      {
        Interop.TransformComponent_GetScale(Entity.Id, out Vector3 scale);
        return scale;
      }
      set
      {
        Interop.TransformComponent_SetScale(Entity.Id, ref value);
      }
    }

    public Vector3 GetForward()
    {
      Interop.TransformComponent_GetForward(Entity.Id, out Vector3 forward);
      return forward;
    }

    public Vector3 GetRight()
    {
      Interop.TransformComponent_GetRight(Entity.Id, out Vector3 right);
      return right;
    }

    public Vector3 GetUp()
    {
      Interop.TransformComponent_GetUp(Entity.Id, out Vector3 up);
      return up;
    }
  }
}
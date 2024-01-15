using System;
using System.Runtime.InteropServices;

namespace EveEngine
{
  public delegate void BoxColliderOnTriggerDelegate(string otherName);

  public class BoxCollider : Component
  {
    public string Name
    {
      get => Entity.Name;
    }

    public bool IsTrigger
    {
      get => Interop.BoxCollider_GetIsTrigger(Entity.Id);
      set => Interop.BoxCollider_SetIsTrigger(Entity.Id, value);
    }

    public Vector3 LocalPosition
    {
      get
      {
        Interop.BoxCollider_GetLocalPosition(Entity.Id, out Vector3 position);
        return position;
      }
      set => Interop.BoxCollider_SetLocalPosition(Entity.Id, ref value);
    }

    public Vector3 LocalScale
    {
      get
      {
        Interop.BoxCollider_GetLocalScale(Entity.Id, out Vector3 scale);
        return scale;
      }
      set => Interop.BoxCollider_SetLocalScale(Entity.Id, ref value);
    }

    public BoxColliderOnTriggerDelegate OnTrigger
    {
      get
      {
        Interop.BoxCollider_GetOnTrigger(Entity.Id, out IntPtr onTrigger);
        if (onTrigger != IntPtr.Zero)
        {
          return Marshal.GetDelegateForFunctionPointer<BoxColliderOnTriggerDelegate>(onTrigger);
        }

        return null;
      }
      set => Interop.BoxCollider_SetOnTrigger(Entity.Id, Marshal.GetFunctionPointerForDelegate<BoxColliderOnTriggerDelegate>(value));
    }

  }
}
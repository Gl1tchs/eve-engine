using System;
using System.Runtime.InteropServices;

namespace EveEngine
{
  /// <summary>
  /// Delegate which will be triggered when two BoxCollider objects collide.
  /// </summary>
  /// <param name="otherName"></param>
  public delegate void BoxColliderOnTriggerDelegate(string otherName);

  /// <summary>
  /// Class representing an box collider physics object on scene,
  /// which can make objects trigger when collide.
  /// </summary>
  public class BoxCollider : Component
  {
    /// <summary>
    /// Returns <see cref="Entity.Name"/>.
    /// </summary>
    public string Name
    {
      get => Entity.Name;
    }

    /// <summary>
    /// Can trigger collision events.
    /// </summary>
    public bool IsTrigger
    {
      get => Interop.BoxCollider_GetIsTrigger(Entity.Id);
      set => Interop.BoxCollider_SetIsTrigger(Entity.Id, value);
    }

    /// <summary>
    /// Local position of the collider.
    /// Later on this value will be adding to the transform of <see cref="Entity.Transform"/>.
    /// </summary>
    public Vector3 LocalPosition
    {
      get
      {
        Interop.BoxCollider_GetLocalPosition(Entity.Id, out Vector3 position);
        return position;
      }
      set => Interop.BoxCollider_SetLocalPosition(Entity.Id, ref value);
    }

    /// <summary>
    /// Local position of the collider.
    /// Later on this value will be using instead of <see cref="Entity.Transform"/>.
    /// </summary>
    public Vector3 LocalScale
    {
      get
      {
        Interop.BoxCollider_GetLocalScale(Entity.Id, out Vector3 scale);
        return scale;
      }
      set => Interop.BoxCollider_SetLocalScale(Entity.Id, ref value);
    }

    /// <summary>
    /// Delegate which will be triggered if <see cref="IsTrigger"/> set to true and two objects collide.
    /// </summary>
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
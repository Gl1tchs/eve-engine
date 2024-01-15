using System;

namespace EveEngine
{
  public class Entity
  {
    public ulong Id { get; private set; }
    public string Name
    {
      get => Interop.Entity_GetName(Id);
    }

    public Transform Transform { get; private set; }

    public static Entity InvalidEntity => new Entity();

    public Entity() { Id = 0; }

    public Entity(ulong id)
    {
      Id = id;
      Transform = GetComponent<Transform>();
    }

    protected virtual void OnCreate() { }

    protected virtual void OnUpdate(float ds) { }

    protected virtual void OnDestroy() { }
     
    public void Destroy()
    {
      // this will destroy this object as well
      Interop.Entity_Destroy(Id);
    }

    public bool HasComponent<T>() where T : Component, new()
    {
      return Interop.Entity_HasComponent(Id, typeof(T));
    }

    public T GetComponent<T>() where T : Component, new()
    {
      if (!HasComponent<T>())
        return null;

      T component = new T() { Entity = this };
      return component;
    }

    public T AddComponent<T>() where T : Component, new()
    {
      var componentType = typeof(T);

      if (HasComponent<T>())
      {
        throw new DuplicateComponentException(String.Format("Entity already has component of: {}", componentType.FullName));
      }

      Interop.Entity_AddComponent(Id, componentType);
      return GetComponent<T>();
    }

    public Entity TryGetEntityByName(string name)
    {
      ulong entityId = Interop.Entity_TryGetEntityByName(name);
      return new Entity(entityId);
    }

    public T As<T>() where T : Entity, new()
    {
      object instance = Interop.GetScriptInstance(Id);
      return instance as T;
    }

    public T Instantiate<T>(string name, Vector3 position, Vector3 rotation, Vector3 scale) where T : Entity, new()
    {
      ulong createdId = Interop.Entity_Instantiate(name, ref position, ref rotation);
      Interop.Entity_AssignScript(createdId, typeof(T).FullName);

      Entity createdEntity = new Entity(createdId);
      return createdEntity as T;
    }

    public T Instantiate<T>(string name, Vector3 position, Vector3 rotation) where T : Entity, new()
    {
      return Instantiate<T>(name, position, rotation, Vector3.One);
    }

    public T Instantiate<T>(string name, Vector3 position) where T : Entity, new()
    {
      return Instantiate<T>(name, position, Vector3.Zero, Vector3.One);
    }

    public T Instantiate<T>(string name) where T : Entity, new()
    {
      return Instantiate<T>(name, Vector3.Zero, Vector3.Zero, Vector3.One);
    }

    public Entity Instantiate(string name, Vector3 position, Vector3 rotation, Vector3 scale)
    {
      return Instantiate<Entity>(name, position, rotation, scale);
    }

    public Entity Instantiate(string name, Vector3 position, Vector3 rotation)
    {
      return Instantiate<Entity>(name, position, rotation, Vector3.One);
    }

    public Entity Instantiate(string name, Vector3 position)
    {
      return Instantiate<Entity>(name, position, Vector3.Zero, Vector3.One);
    }

    public Entity Instantiate(string name)
    {
      return Instantiate<Entity>(name, Vector3.Zero, Vector3.Zero, Vector3.One);
    }

    public static implicit operator bool(Entity entity)
    {
      return entity.Id != 0;
    }
  }
}

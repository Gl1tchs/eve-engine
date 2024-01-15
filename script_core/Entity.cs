namespace EveEngine
{
  /// <summary>
  /// Represents an entity within the scene.
  /// </summary>
  public class Entity
  {
    /// <summary>
    /// Unique 64 bit entity UUID.
    /// </summary>
    public ulong Id { get; private set; }

    /// <summary>
    /// Entity name of the scene.
    /// </summary>
    public string Name
    {
      get => Interop.Entity_GetName(Id);
    }

    /// <summary>
    /// Transform of the entity.
    /// </summary>
    public Transform Transform { get; private set; }

    /// <summary>
    /// Entity with an id of 0.
    /// </summary>
    public static Entity InvalidEntity => new();

    /// <summary>
    /// Initializes a new instance of the <see cref="Entity"/> class with an invalid ID (0).
    /// </summary>
    public Entity() { Id = 0; }

    /// <summary>
    /// Initializes a new instance of the <see cref="Entity"/> class with a specified ID.
    /// </summary>
    /// <param name="id">The unique identifier to assign to the entity.</param>
    public Entity(ulong id)
    {
      Id = id;
      Transform = GetComponent<Transform>();
    }

    /// <summary>
    /// Called on entity creation.
    /// </summary>
    protected virtual void OnCreate() { }

    /// <summary>
    /// Called every frame.
    /// </summary>
    /// <param name="ds">Delta seconds.</param>
    protected virtual void OnUpdate(float ds) { }

    /// <summary>
    /// Called on entity destroy events.
    /// </summary>
    protected virtual void OnDestroy() { }

    /// <summary>
    /// Destroys entity from scene.
    /// </summary>
    public void Destroy()
    {
      // this will destroy this object as well
      Interop.Entity_Destroy(Id);
    }

    /// <summary>
    /// Find if entity has given component.
    /// </summary>
    /// <typeparam name="T">Component to look for.</typeparam>
    /// <returns><c>true</c> if found <c>else</c> otherwise.</returns>
    public bool HasComponent<T>() where T : Component, new()
    {
      return Interop.Entity_HasComponent(Id, typeof(T));
    }

    /// <summary>
    /// Get given component from entity.
    /// </summary>
    /// <typeparam name="T">Component to look for.</typeparam>
    /// <returns>Component reference if found <c>null</c> otherwise.</returns>
    public T GetComponent<T>() where T : Component, new()
    {
      if (!HasComponent<T>())
        return null;

      T component = new() { Entity = this };
      return component;
    }

    /// <summary>
    /// Add component of type T to the entity.
    /// Will throw exception if component already exists.
    /// </summary>
    /// <typeparam name="T">Component to add.</typeparam>
    /// <returns>Added component reference.</returns>
    /// <exception cref="DuplicateComponentException">Will be thrown if component already exists.</exception>
    public T AddComponent<T>() where T : Component, new()
    {
      var componentType = typeof(T);

      if (HasComponent<T>())
      {
        throw new DuplicateComponentException(string.Format("Entity already has component of: {0}", componentType.FullName));
      }

      Interop.Entity_AddComponent(Id, componentType);
      return GetComponent<T>();
    }

    /// <summary>
    /// Find entity of given name.
    /// If scene has multiple entities of same name
    /// it will return the first occurance.
    /// </summary>
    /// <param name="name">Entity name to look for.</param>
    /// <returns>Entity reference if found Entity. <c>InvalidEntity</c> otherwise.</returns>
    public Entity TryGetEntityByName(string name)
    {
      ulong entityId = Interop.Entity_TryGetEntityByName(name);
      return new Entity(entityId);
    }

    /// <summary>
    /// Get entity as a script instance.
    /// </summary>
    /// <typeparam name="T">The entity script instance class to cast.</typeparam>
    /// <returns>Script instance of the entity.</returns>
    public T As<T>() where T : Entity, new()
    {
      object instance = Interop.GetScriptInstance(Id);
      return instance as T;
    }

    /// <summary>
    /// Instantiates new entity on the scene with a script instance.
    /// </summary>
    /// <typeparam name="T">Entity script class to create with.</typeparam>
    /// <param name="name">The entity name of the scene.</param>
    /// <param name="position">Position of the entity transform.</param>
    /// <param name="rotation">Rotation of the entity transform.</param>
    /// <param name="scale">Scale of the entity transform.</param>
    /// <returns>New created entity instance.</returns>
    public T Instantiate<T>(string name, Vector3 position, Vector3 rotation, Vector3 scale) where T : Entity, new()
    {
      ulong createdId = Interop.Entity_Instantiate(name, ref position, ref rotation);

      // Only assign script if has it.
      if (typeof(T) != typeof(Entity))
      {
        Interop.Entity_AssignScript(createdId, typeof(T).FullName);
      }

      Entity createdEntity = new(createdId);
      return createdEntity as T;
    }

    /// <summary>
    /// Instantiates new entity on the scene with a script instance.
    /// </summary>
    /// <typeparam name="T">Entity script class to create with.</typeparam>
    /// <param name="name">The entity name of the scene.</param>
    /// <param name="position">Position of the entity transform.</param>
    /// <param name="rotation">Rotation of the entity transform.</param>
    /// <returns>New created entity instance.</returns>
    public T Instantiate<T>(string name, Vector3 position, Vector3 rotation) where T : Entity, new()
    {
      return Instantiate<T>(name, position, rotation, Vector3.One);
    }

    /// <summary>
    /// Instantiates new entity on the scene with a script instance.
    /// </summary>
    /// <typeparam name="T">Entity script class to create with.</typeparam>
    /// <param name="name">The entity name of the scene.</param>
    /// <param name="position">Position of the entity transform.</param>
    /// <returns>New created entity instance.</returns>
    public T Instantiate<T>(string name, Vector3 position) where T : Entity, new()
    {
      return Instantiate<T>(name, position, Vector3.Zero, Vector3.One);
    }

    /// <summary>
    /// Instantiates new entity on the scene with a script instance.
    /// </summary>
    /// <typeparam name="T">Entity script class to create with.</typeparam>
    /// <param name="name">The entity name of the scene.</param>
    /// <returns>New created entity instance.</returns>
    public T Instantiate<T>(string name) where T : Entity, new()
    {
      return Instantiate<T>(name, Vector3.Zero, Vector3.Zero, Vector3.One);
    }

    /// <summary>
    /// Instantiates new entity on the scene.
    /// </summary>
    /// <param name="name">The entity name of the scene.</param>
    /// <param name="position">Position of the entity transform.</param>
    /// <param name="rotation">Rotation of the entity transform.</param>
    /// <param name="scale">Scaleof the entity transform.</param>
    /// <returns>New created entity instance.</returns>
    public Entity Instantiate(string name, Vector3 position, Vector3 rotation, Vector3 scale)
    {
      return Instantiate<Entity>(name, position, rotation, scale);
    }

    /// <summary>
    /// Instantiates new entity on the scene.
    /// </summary>
    /// <param name="name">The entity name of the scene.</param>
    /// <param name="position">Position of the entity transform.</param>
    /// <param name="rotation">Rotation of the entity transform.</param>
    /// <returns>New created entity instance.</returns>
    public Entity Instantiate(string name, Vector3 position, Vector3 rotation)
    {
      return Instantiate<Entity>(name, position, rotation, Vector3.One);
    }

    /// <summary>
    /// Instantiates new entity on the scene.
    /// </summary>
    /// <param name="name">The entity name of the scene.</param>
    /// <param name="position">Position of the entity transform.</param>
    /// <returns>New created entity instance.</returns>
    public Entity Instantiate(string name, Vector3 position)
    {
      return Instantiate<Entity>(name, position, Vector3.Zero, Vector3.One);
    }

    /// <summary>
    /// Instantiates new entity on the scene.
    /// </summary>
    /// <param name="name">The entity name of the scene.</param>
    /// <returns>New created entity instance.</returns>
    public Entity Instantiate(string name)
    {
      return Instantiate<Entity>(name, Vector3.Zero, Vector3.Zero, Vector3.One);
    }

    /// <summary>
    /// Determines whether the specified object is equal to the current entity.
    /// </summary>
    /// <param name="obj">The object to compare with the current entity.</param>
    /// <returns>
    /// <c>true</c> if the specified object is equal to the current entity; otherwise, <c>false</c>.
    /// </returns>
    /// <remarks>
    /// Two entities are considered equal if they have the same identifier (Id).
    /// </remarks>
    public override bool Equals(object obj)
    {
      if ((obj == null) || !this.GetType().Equals(obj.GetType()))
      {
        return false;
      }
      else
      {
        Entity entity = (Entity)obj;
        return Id == entity.Id;
      }
    }

    /// <summary>
    /// Serves as a hash function for the current entity.
    /// </summary>
    /// <returns>A hash code for the current entity.</returns>
    public override int GetHashCode()
    {
      return Id.GetHashCode();
    }

    /// <summary>
    /// Implicitly converts an entity to a boolean value.
    /// </summary>
    /// <param name="entity">The entity to convert.</param>
    /// <returns>
    /// <c>true</c> if the entity's identifier (Id) is not equal to 0; otherwise, <c>false</c>.
    /// </returns>
    public static implicit operator bool(Entity entity)
    {
      return entity.Id != 0;
    }
  }
}

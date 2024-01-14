using System;

namespace EveEngine
{
	public class Entity
	{
		public ulong Id { get; private set; }
		public Transform Transform { get; private set; }

		public static Entity InvalidEntity => new Entity();

		protected Entity() { Id = 0; }

		internal Entity(ulong id)
		{
			Id = id;
			Transform = GetComponent<Transform>();
		}

		protected virtual void OnCreate() { }

		protected virtual void OnUpdate(float ds) { }

		protected virtual void OnDestroy() { }

		public bool HasComponent<T>() where T : Component, new()
		{
			return InternalCalls.Entity_HasComponent(Id, typeof(T));
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Entity = this };
			return component;
		}

		public Entity TryGetEntityByName(string name)
		{
			ulong entityId = InternalCalls.Entity_TryGetEntityByName(name);
			return new Entity(entityId);
		}

		public T As<T>() where T : Entity, new()
		{
			object instance = InternalCalls.GetScriptInstance(Id);
			return instance as T;
		}

		public static implicit operator bool(Entity entity)
		{
			return entity.Id != 0;
		}
	}
}

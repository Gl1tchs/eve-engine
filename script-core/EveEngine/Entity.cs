using System;
using System.Runtime.CompilerServices;

namespace EveEngine
{
	public class Entity
	{
		protected Entity() { id = 0; }

		internal Entity(ulong id)
		{
			this.id = id;
		}

		public readonly ulong id;

		// TODO store directly transform
		public Vector3 position
		{
			get
			{
				InternalCalls.TransformComponent_GetPosition(id, out Vector3 result);
				return result;
			}
			set
			{
				InternalCalls.TransformComponent_SetPosition(id, ref value);
			}
		}

		public bool HasComponent<T>() where T : Component, new()
		{
			Type component_type = typeof(T);
			return InternalCalls.Entity_HasComponent(id, component_type);
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { entity = this };
			return component;
		}

		public Entity FindEntityByName(string name)
		{
			ulong entity_id = InternalCalls.Entity_FindEntityByName(name);
			if (entity_id == 0)
				return null;

			return new Entity(entity_id);
		}

		public T As<T>() where T : Entity, new()
		{
			object instance = InternalCalls.GetScriptInstance(id);
			return instance as T;
		}

	}

}

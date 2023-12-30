using System;
using System.Runtime.CompilerServices;

namespace EveEngine
{
	public class ScriptEntity
	{
		public ulong Id { get; private set; }

		public static ScriptEntity InvalidEntity = new ScriptEntity();

		protected ScriptEntity() { Id = 0; }

		internal ScriptEntity(ulong id)
		{
			this.Id = id;
		}


		// TODO store directly transform
		public Vector3 Position
		{
			get
			{
				InternalCalls.TransformComponent_GetPosition(Id, out Vector3 result);
				return result;
			}
			set
			{
				InternalCalls.TransformComponent_SetPosition(Id, ref value);
			}
		}

		public bool HasComponent<T>() where T : Component, new()
		{
			Type component_type = typeof(T);
			return InternalCalls.Entity_HasComponent(Id, component_type);
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Entity = this };
			return component;
		}

		public ScriptEntity FindEntityByName(string name)
		{
			ulong entity_id = InternalCalls.Entity_FindEntityByName(name);
			return new ScriptEntity(entity_id);
		}

		public T As<T>() where T : ScriptEntity, new()
		{
			object instance = InternalCalls.GetScriptInstance(Id);
			return instance as T;
		}

		public static implicit operator bool(ScriptEntity entity)
		{
			return entity.Id != 0;
		}
	}
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EveEngine
{
	public abstract class Component
	{
		public ScriptEntity Entity { get; internal set; }
	}

	public class CameraComponent : Component
	{
		public OrthographicCamera OrthoCamera
		{
			get
			{
				OrthoCamera.SetEntity(Entity);
				return OrthoCamera;
			}
		}
		public PerspectiveCamera PerspCamera
		{
			get
			{
				PerspCamera.SetEntity(Entity);
				return PerspCamera;
			}
		}
		public bool IsOrthographic
		{
			get
			{
				InternalCalls.CameraComponent_GetIsOrthographic(Entity.Id, out bool isOrthographic);
				return isOrthographic;
			}
			set
			{
				InternalCalls.CameraComponent_SetIsOrthographic(Entity.Id, ref value);
			}
		}
		public bool IsPrimary
		{
			get
			{
				InternalCalls.CameraComponent_GetIsPrimary(Entity.Id, out bool isPrimary);
				return isPrimary;
			}
			set
			{
				InternalCalls.CameraComponent_SetIsPrimary(Entity.Id, ref value);
			}
		}
		public bool IsFixedAspectRatio
		{
			get
			{
				InternalCalls.CameraComponent_GetIsFixedAspectRatio(Entity.Id, out bool isFixedAspectRatio);
				return isFixedAspectRatio;
			}
			set
			{
				InternalCalls.CameraComponent_SetIsFixedAspectRatio(Entity.Id, ref value);
			}
		}
	}

	public class Material : Component
	{
		public Vector3 Albedo
		{
			get
			{
				InternalCalls.Material_GetAlbedo(Entity.Id, out Vector3 albedo);
				return albedo;
			}
			set
			{
				InternalCalls.Material_SetAlbedo(Entity.Id, ref value);
			}
		}
		public float Metallic
		{
			get
			{
				InternalCalls.Material_GetMetallic(Entity.Id, out float metallic);
				return metallic;
			}
			set
			{
				InternalCalls.Material_SetMetallic(Entity.Id, ref value);
			}
		}
		public float Roughness
		{
			get
			{
				InternalCalls.Material_GetRoughness(Entity.Id, out float roughness);
				return roughness;
			}
			set
			{
				InternalCalls.Material_SetRoughness(Entity.Id, ref value);
			}
		}
		public float Ao
		{
			get
			{
				InternalCalls.Material_GetAO(Entity.Id, out float ao);
				return ao;
			}
			set
			{
				InternalCalls.Material_SetAO(Entity.Id, ref value);
			}
		}
	}
}

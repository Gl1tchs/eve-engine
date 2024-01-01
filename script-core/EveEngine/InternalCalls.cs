using System;
using System.Runtime.CompilerServices;

namespace EveEngine
{
	static partial class InternalCalls
	{
		#region Entity
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityId, Type component_type);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static ulong Entity_TryGetEntityByName(string name);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static object GetScriptInstance(ulong entityId);
		#endregion

		#region TransformComponent
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetPosition(ulong entityId, out Vector3 position);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetPosition(ulong entityId, ref Vector3 position);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetRotation(ulong entityId, out Vector3 rotation);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetRotation(ulong entityId, ref Vector3 rotation);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetScale(ulong entityId, out Vector3 scale);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetScale(ulong entityId, ref Vector3 scale);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetForward(ulong entityId, out Vector3 forward);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetRight(ulong entityId, out Vector3 right);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetUp(ulong entityId, out Vector3 up);

		#endregion

		#region CameraComponent
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_GetAspectRatio(ulong entityId, out float aspectRatio);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_SetAspectRatio(ulong entityId, ref float aspectRatio);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_GetZoomLevel(ulong entityId, out float zoomLevel);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_SetZoomLevel(ulong entityId, ref float zoomLevel);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_GetNearClip(ulong entityId, out float nearClip);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_SetNearClip(ulong entityId, ref float nearClip);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_GetFarClip(ulong entityId, out float farClip);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_OrthographicCamera_SetFarClip(ulong entityId, ref float farClip);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_GetAspectRatio(ulong entityId, out float aspectRatio);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_SetAspectRatio(ulong entityId, ref float aspectRatio);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_GetFov(ulong entityId, out float fov);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_SetFov(ulong entityId, ref float fov);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_GetNearClip(ulong entityId, out float nearClip);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_SetNearClip(ulong entityId, ref float nearClip);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_GetFarClip(ulong entityId, out float farClip);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_PerspectiveCamera_SetFarClip(ulong entityId, ref float farClip);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_GetIsOrthographic(ulong entityId, out bool isOrthographic);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetIsOrthographic(ulong entityId, ref bool isOrthographic);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_GetIsPrimary(ulong entityId, out bool isPrimary);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetIsPrimary(ulong entityId, ref bool isPrimary);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_GetIsFixedAspectRatio(ulong entityId, out bool isFixedAspectRatio);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetIsFixedAspectRatio(ulong entityId, ref bool isFixedAspectRatio);
		#endregion

		#region Material
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_GetAlbedo(ulong entityId, out Vector3 albedo);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_SetAlbedo(ulong entityId, ref Vector3 albedo);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_GetMetallic(ulong entityId, out float metallic);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_SetMetallic(ulong entityId, ref float metallic);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_GetRoughness(ulong entityId, out float roughness);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_SetRoughness(ulong entityI, ref float roughness);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_GetAO(ulong entityId, out float ao);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Material_SetAO(ulong entityId, ref float ao);
		#endregion

		#region SceneManager
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SceneManager_SetActive(int index);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SceneManager_GetActiveIndex();
		#endregion

		#region Input
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyPressed(KeyCode keyCode);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyReleased(KeyCode keyCode);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonPressed(MouseCode mouseCode);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonReleased(MouseCode mouseCode);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Input_GetMousePosition(out Vector2 position);
		#endregion
	}
}

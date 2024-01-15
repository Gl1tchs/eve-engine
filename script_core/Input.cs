namespace EveEngine
{
	public static class Input
	{
		public static bool IsKeyPressed(KeyCode keyCode)
		{
			return Interop.Input_IsKeyPressed(keyCode);
		}

		public static bool IsKeyPressed(string key)
		{
			return Interop.Input_IsKeyPressedString(key);
		}

		public static bool IsKeyReleased(KeyCode keyCode)
		{
			return Interop.Input_IsKeyReleased(keyCode);
		}

		public static bool IsKeyReleased(string key)
		{
			return Interop.Input_IsKeyReleasedString(key);
		}

		public static bool IsMouseButtonPressed(MouseCode mouseCode)
		{
			return Interop.Input_IsMouseButtonPressed(mouseCode);
		}

		public static bool IsMouseButtonPressed(string key)
		{
			return Interop.Input_IsMouseButtonPressedString(key);
		}

		public static bool IsMouseButtonReleased(MouseCode mouseCode)
		{
			return Interop.Input_IsMouseButtonReleased(mouseCode);
		}

		public static bool IsMouseButtonReleased(string key)
		{
			return Interop.Input_IsMouseButtonReleasedString(key);
		}

		public static Vector2 GetMousePosition()
		{
			Interop.Input_GetMousePosition(out Vector2 position);
			return position;
		}
	}
}

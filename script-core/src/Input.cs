namespace EveEngine
{
	public static class Input
	{
		public static bool IsKeyPressed(KeyCode keyCode)
		{
			return InternalCalls.Input_IsKeyPressed(keyCode);
		}

		public static bool IsKeyPressed(string key)
		{
			return InternalCalls.Input_IsKeyPressedString(key);
		}

		public static bool IsKeyReleased(KeyCode keyCode)
		{
			return InternalCalls.Input_IsKeyReleased(keyCode);
		}

		public static bool IsKeyReleased(string key)
		{
			return InternalCalls.Input_IsKeyReleasedString(key);
		}

		public static bool IsMouseButtonPressed(MouseCode mouseCode)
		{
			return InternalCalls.Input_IsMouseButtonPressed(mouseCode);
		}

		public static bool IsMouseButtonPressed(string key)
		{
			return InternalCalls.Input_IsMouseButtonPressedString(key);
		}

		public static bool IsMouseButtonReleased(MouseCode mouseCode)
		{
			return InternalCalls.Input_IsMouseButtonReleased(mouseCode);
		}

		public static bool IsMouseButtonReleased(string key)
		{
			return InternalCalls.Input_IsMouseButtonReleasedString(key);
		}

		public static Vector2 GetMousePosition()
		{
			InternalCalls.Input_GetMousePosition(out Vector2 position);
			return position;
		}
	}
}

namespace EveEngine
{
	public static class Input
	{
		public static bool IsKeyPressed(KeyCode keyCode)
		{
			return InternalCalls.Input_IsKeyPressed(keyCode);
		}

		public static bool IsKeyReleased(KeyCode keyCode)
		{
			return InternalCalls.Input_IsKeyReleased(keyCode);
		}

		public static bool IsMouseButtonPressed(MouseCode mouseCode)
		{
			return InternalCalls.Input_IsMouseButtonPressed(mouseCode);
		}

		public static bool IsMouseButtonReleased(MouseCode mouseCode)
		{
			return InternalCalls.Input_IsMouseButtonReleased(mouseCode);
		}

		public static Vector2 GetMousePosition()
		{
			InternalCalls.Input_GetMousePosition(out Vector2 position);
			return position;
		}
	}
}

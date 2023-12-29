namespace EveEngine
{
	public static class Input
	{
		public static bool IsKeyPressed(KeyCode key_code)
		{
			return InternalCalls.Input_IsKeyPressed(key_code);
		}

		public static bool IsKeyReleased(KeyCode key_code)
		{
			return InternalCalls.Input_IsKeyReleased(key_code);
		}
	}
}

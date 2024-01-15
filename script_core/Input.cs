namespace EveEngine
{
  /// <summary>
  /// A static class providing methods to query input state, such as key and mouse button presses.
  /// </summary>
  public static class Input
  {
    /// <summary>
    /// Checks if the specified keyboard key is currently pressed.
    /// </summary>
    /// <param name="keyCode">The KeyCode of the key to check.</param>
    /// <returns>True if the key is currently pressed, otherwise false.</returns>
    public static bool IsKeyPressed(KeyCode keyCode)
    {
      return Interop.Input_IsKeyPressed(keyCode);
    }

    /// <summary>
    /// Checks if the specified keyboard key is currently pressed.
    /// </summary>
    /// <param name="key">The string representation of the key to check.</param>
    /// <returns>True if the key is currently pressed, otherwise false.</returns>
    public static bool IsKeyPressed(string key)
    {
      return Interop.Input_IsKeyPressedString(key);
    }

    /// <summary>
    /// Checks if the specified keyboard key has been released since the last frame.
    /// </summary>
    /// <param name="keyCode">The KeyCode of the key to check.</param>
    /// <returns>True if the key has been released, otherwise false.</returns>
    public static bool IsKeyReleased(KeyCode keyCode)
    {
      return Interop.Input_IsKeyReleased(keyCode);
    }

    /// <summary>
    /// Checks if the specified keyboard key has been released since the last frame.
    /// </summary>
    /// <param name="key">The string representation of the key to check.</param>
    /// <returns>True if the key has been released, otherwise false.</returns>
    public static bool IsKeyReleased(string key)
    {
      return Interop.Input_IsKeyReleasedString(key);
    }

    /// <summary>
    /// Checks if the specified mouse button is currently pressed.
    /// </summary>
    /// <param name="mouseCode">The MouseCode of the mouse button to check.</param>
    /// <returns>True if the mouse button is currently pressed, otherwise false.</returns>
    public static bool IsMouseButtonPressed(MouseCode mouseCode)
    {
      return Interop.Input_IsMouseButtonPressed(mouseCode);
    }

    /// <summary>
    /// Checks if the specified mouse button is currently pressed.
    /// </summary>
    /// <param name="key">The string representation of the mouse button to check.</param>
    /// <returns>True if the mouse button is currently pressed, otherwise false.</returns>
    public static bool IsMouseButtonPressed(string key)
    {
      return Interop.Input_IsMouseButtonPressedString(key);
    }

    /// <summary>
    /// Checks if the specified mouse button has been released since the last frame.
    /// </summary>
    /// <param name="mouseCode">The MouseCode of the mouse button to check.</param>
    /// <returns>True if the mouse button has been released, otherwise false.</returns>
    public static bool IsMouseButtonReleased(MouseCode mouseCode)
    {
      return Interop.Input_IsMouseButtonReleased(mouseCode);
    }

    /// <summary>
    /// Checks if the specified mouse button has been released since the last frame.
    /// </summary>
    /// <param name="key">The string representation of the mouse button to check.</param>
    /// <returns>True if the mouse button has been released, otherwise false.</returns>
    public static bool IsMouseButtonReleased(string key)
    {
      return Interop.Input_IsMouseButtonReleasedString(key);
    }

    /// <summary>
    /// Retrieves the current mouse position.
    /// </summary>
    /// <returns>A Vector2 representing the current mouse position.</returns>
    public static Vector2 GetMousePosition()
    {
      Interop.Input_GetMousePosition(out Vector2 position);
      return position;
    }
  }
}

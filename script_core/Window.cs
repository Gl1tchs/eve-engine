namespace EveEngine
{
  /// <summary>
  /// Enum representing the cusor mode.
  /// </summary>
  public enum CursorMode
  {
#pragma warning disable CS1591
    Normal,
    Hidden,
    Disabled,
    Captured,
#pragma warning restore CS1591
  }

  /// <summary>
  /// Static class representing the application window.
  /// </summary>
  public static class Window
  {
    /// <summary>
    /// Mode of the cursor on the window.
    /// </summary>
    public static CursorMode CursorMode
    {
      get => Interop.Window_GetCursorMode();
      set => Interop.Window_SetCursorMode(value);
    }
  }
}
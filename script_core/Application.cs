namespace EveEngine
{
  /// <summary>
  /// Static class representing the OS application.
  /// </summary>
  public static class Application
  {
    /// <summary>
    /// Exits application safely on the next frame.
    /// </summary>
    /// <disclaimer>
    /// It won't quit from the editor application process
    /// but closes the editor itself.
    /// </disclaimer>
    public static void Quit()
    {
      Interop.Application_Quit();
    }
  }
}
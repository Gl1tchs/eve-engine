namespace EveEngine
{
  /// <summary>
  /// A static class providing functionality to manage scenes in the game.
  /// </summary>
  public static class SceneManager
  {
    /// <summary>
    /// Sets the specified scene as the active scene.
    /// </summary>
    /// <param name="index">The index of the scene to set as active.</param>
    public static void SetActive(int index)
    {
      Interop.SceneManager_SetActive(index);
    }

    /// <summary>
    /// Gets the index of the currently active scene.
    /// </summary>
    /// <returns>The index of the currently active scene.</returns>
    public static int GetActiveIndex()
    {
      return Interop.SceneManager_GetActiveIndex();
    }
  }
}

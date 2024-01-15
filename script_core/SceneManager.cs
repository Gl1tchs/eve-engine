namespace EveEngine
{
  public static class SceneManager
  {
    public static void SetActive(int index)
    {
      Interop.SceneManager_SetActive(index);
    }

    public static int GetActiveIndex(int index)
    {
      return Interop.SceneManager_GetActiveIndex();
    }
  }
}
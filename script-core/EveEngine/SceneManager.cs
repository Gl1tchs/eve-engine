namespace EveEngine
{
  public static class SceneManager
  {
    public static void SetActive(int index)
    {
      InternalCalls.SceneManager_SetActive(index);
    }

    public static int GetActiveIndex(int index)
    {
      return InternalCalls.SceneManager_GetActiveIndex();
    }
  }
}
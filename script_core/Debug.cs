namespace EveEngine
{
  public static class Debug
  {
    public static void Log(string message)
    {
      Interop.Debug_Log(message);
    }

    public static void LogInfo(string message)
    {
      Interop.Debug_LogInfo(message);
    }

    public static void LogWarning(string message)
    {
      Interop.Debug_LogWarning(message);
    }

    public static void LogError(string message)
    {
      Interop.Debug_LogError(message);
    }

    public static void LogFatal(string message)
    {
      Interop.Debug_LogFatal(message);
    }
  }
}
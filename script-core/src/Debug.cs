namespace EveEngine
{
  public static class Debug
  {
    public static void Log(string message)
    {
      InternalCalls.Debug_Log(message);
    }

    public static void LogInfo(string message)
    {
      InternalCalls.Debug_LogInfo(message);
    }

    public static void LogWarning(string message)
    {
      InternalCalls.Debug_LogWarning(message);
    }

    public static void LogError(string message)
    {
      InternalCalls.Debug_LogError(message);
    }

    public static void LogFatal(string message)
    {
      InternalCalls.Debug_LogFatal(message);
    }
  }
}
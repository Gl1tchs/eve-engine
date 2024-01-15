namespace EveEngine
{
  public class ScriptComponent : Component
  {
    public string ClassName
    {
      get => Interop.ScriptComponent_GetClassName(Entity.Id);
    }
  }
}
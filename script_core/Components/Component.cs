namespace EveEngine
{
  /// <summary>
  /// Abstract class representing base component type with holding entity id in it.
  /// </summary>
  public abstract class Component
  {
    /// <summary>
    /// Entity object which is attached to the component.
    /// </summary>
    public Entity Entity { get; internal set; }
  }
}

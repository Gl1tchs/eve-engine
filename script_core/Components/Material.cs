namespace EveEngine
{
  /// <summary>
  /// Represents a material component attached to an entity.
  /// </summary>
  public class Material : Component
  {
    /// <summary>
    /// Gets or sets the albedo color of the material.
    /// </summary>
    public Color Albedo
    {
      get
      {
        Interop.Material_GetAlbedo(Entity.Id, out Color albedo);
        return albedo;
      }
      set => Interop.Material_SetAlbedo(Entity.Id, ref value);
    }
  }
}

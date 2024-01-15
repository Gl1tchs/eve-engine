namespace EveEngine
{
  public class Material : Component
  {
    public Color Albedo
    {
      get
      {
        Interop.Material_GetAlbedo(Entity.Id, out Color albedo);
        return albedo;
      }
      set
      {
        Interop.Material_SetAlbedo(Entity.Id, ref value);
      }
    }
  }
}
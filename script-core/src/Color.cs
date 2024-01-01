using System;

namespace EveEngine
{
  public struct Color
  {
    public float r, g, b, a;

    public static Color Black => new Color(0, 0, 0, 1);
    public static Color White => new Color(1, 1, 1, 1);
    public static Color Red => new Color(1, 0, 0, 1);
    public static Color Green => new Color(0, 1, 0, 1);
    public static Color Blue => new Color(0, 0, 1, 1);
    public static Color Yellow => new Color(1, 1, 0, 1);
    public static Color Cyan => new Color(0, 1, 1, 1);
    public static Color Magenta => new Color(1, 0, 1, 1);
    public static Color Gray => new Color(0.5f, 0.5f, 0.5f, 1);
    public static Color Orange => new Color(1, 0.5f, 0, 1);

    public Color(float value)
    {
      r = value;
      g = value;
      b = value;
      a = 1.0f;
    }

    public Color(float r, float g, float b, float a = 1.0f)
    {
      this.r = r;
      this.g = g;
      this.b = b;
      this.a = a;
    }

    public Color(Vector3 rgb, float a = 1.0f)
    {
      r = rgb.x;
      g = rgb.y;
      b = rgb.z;
      this.a = a;
    }

    public Color(Vector4 rgba)
    {
      r = rgba.x;
      g = rgba.y;
      b = rgba.z;
      a = rgba.w;
    }

    public Vector3 RGB
    {
      get
      {
        return new Vector3(r, g, b);
      }
      set
      {
        r = value.x;
        g = value.y;
        b = value.z;
      }
    }

    public Vector4 RGBA
    {
      get
      {
        return new Vector4(r, g, b, a);
      }
      set
      {
        r = value.x;
        g = value.y;
        b = value.z;
        a = value.w;
      }
    }

    public string ToHex()
    {
      int r = (int)(this.r * 255);
      int g = (int)(this.g * 255);
      int b = (int)(this.b * 255);
      int a = (int)(this.a * 255);

      return $"#{a:X2}{r:X2}{g:X2}{b:X2}";
    }
  }
}

namespace EveEngine
{
  /// <summary>
  /// Class representing a color value.
  /// </summary>
  public struct Color
  {
    /// <summary>
    /// Red channel of the color.
    /// </summary>
    public float r;

    /// <summary>
    /// Green channel of the color.
    /// </summary>
    public float g;

    /// <summary>
    /// Blue channel of the color.
    /// </summary>
    public float b;

    /// <summary>
    /// Alpha channel of the color.
    /// </summary>
    public float a;

#pragma warning disable CS1591
    public static Color Black => new(0, 0, 0, 1);
    public static Color White => new(1, 1, 1, 1);
    public static Color Red => new(1, 0, 0, 1);
    public static Color Green => new(0, 1, 0, 1);
    public static Color Blue => new(0, 0, 1, 1);
    public static Color Yellow => new(1, 1, 0, 1);
    public static Color Cyan => new(0, 1, 1, 1);
    public static Color Magenta => new(1, 0, 1, 1);
    public static Color Gray => new(0.5f, 0.5f, 0.5f, 1);
    public static Color Orange => new(1, 0.5f, 0, 1);
#pragma warning restore CS1591
    /// <summary>
    /// Initializes a new instance of the <see cref="Color"/> class with all channels set to the specified value.
    /// The alpha channel is set to 1.0f.
    /// </summary>
    /// <param name="value">The value to set for all color channels.</param>
    public Color(float value)
    {
      r = value;
      g = value;
      b = value;
      a = 1.0f;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Color"/> class with specific channel values.
    /// </summary>
    /// <param name="r">Red channel value.</param>
    /// <param name="g">Green channel value.</param>
    /// <param name="b">Blue channel value.</param>
    /// <param name="a">Alpha channel value (default is 1.0f).</param>
    public Color(float r, float g, float b, float a = 1.0f)
    {
      this.r = r;
      this.g = g;
      this.b = b;
      this.a = a;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Color"/> class from a <see cref="Vector3"/> and alpha.
    /// </summary>
    /// <param name="rgb">The <see cref="Vector3"/> representing the RGB channels.</param>
    /// <param name="a">Alpha channel value (default is 1.0f).</param>
    public Color(Vector3 rgb, float a = 1.0f)
    {
      r = rgb.x;
      g = rgb.y;
      b = rgb.z;
      this.a = a;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Color"/> class from a <see cref="Vector4"/>.
    /// </summary>
    /// <param name="rgba">The <see cref="Vector4"/> representing the RGBA channels.</param>
    public Color(Vector4 rgba)
    {
      r = rgba.x;
      g = rgba.y;
      b = rgba.z;
      a = rgba.w;
    }

    /// <summary>
    /// Get RGB Vector3 representation of the color.
    /// </summary>
    public Vector3 RGB
    {
      readonly get => new(r, g, b);
      set
      {
        r = value.x;
        g = value.y;
        b = value.z;
      }
    }

    /// <summary>
    /// Get RGB Vector3 representation of the color.
    /// </summary>
    public Vector4 RGBA
    {
      readonly get => new(r, g, b, a);
      set
      {
        r = value.x;
        g = value.y;
        b = value.z;
        a = value.w;
      }
    }
  }
}

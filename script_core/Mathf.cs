using System;

namespace EveEngine
{
  /// <summary>
  /// A static class providing mathematical utility functions.
  /// </summary>
  public static class Mathf
  {
    /// <summary>
    /// A small value used as a threshold for floating-point comparisons.
    /// </summary>
    public static float EPSILON = 1e-5f;

    /// <summary>
    /// Normalizes the given 2D vector, ensuring its length is 1.
    /// </summary>
    /// <param name="vector">The vector to normalize.</param>
    /// <returns>The normalized vector.</returns>
    /// <exception cref="InvalidOperationException">Thrown when attempting to normalize a zero-length vector.</exception>
    public static Vector2 Normalize(Vector2 vector)
    {
      float length = vector.Length();

      // Check if the length is not close to zero to avoid division by near-zero values
      if (Math.Abs(length) > EPSILON)
        return vector / length;
      else
        throw new InvalidOperationException("Cannot normalize a zero-length vector.");
    }

    /// <summary>
    /// Normalizes the given 3D vector, ensuring its length is 1.
    /// </summary>
    /// <param name="vector">The vector to normalize.</param>
    /// <returns>The normalized vector.</returns>
    /// <exception cref="InvalidOperationException">Thrown when attempting to normalize a zero-length vector.</exception>
    public static Vector3 Normalize(Vector3 vector)
    {
      float length = vector.Length();

      // Check if the length is not close to zero to avoid division by near-zero values
      if (Math.Abs(length) > EPSILON)
        return vector / length;
      else
        throw new InvalidOperationException("Cannot normalize a zero-length vector.");
    }

    /// <summary>
    /// Clamps the value to the specified range.
    /// </summary>
    /// <param name="value">The value to clamp.</param>
    /// <param name="min">Minimum value.</param>
    /// <param name="max">Maximum value.</param>
    /// <returns>The clamped value.</returns>
    public static float Clamp(float value, float min, float max)
    {
      return Math.Max(min, Math.Min(value, max));
    }

    /// <summary>
    /// Clamps the components of a 2D vector to the specified range.
    /// </summary>
    /// <param name="value">The vector to clamp.</param>
    /// <param name="min">The minimum values for each component.</param>
    /// <param name="max">The maximum values for each component.</param>
    /// <returns>The clamped vector.</returns>
    public static Vector2 Clamp(Vector2 value, Vector2 min, Vector2 max)
    {
      float x = Clamp(value.x, min.x, max.x);
      float y = Clamp(value.y, min.y, max.y);
      return new Vector2(x, y);
    }

    /// <summary>
    /// Clamps the components of a 2D vector to the specified range.
    /// </summary>
    /// <param name="value">The vector to clamp.</param>
    /// <param name="min">The minimum values for each component.</param>
    /// <param name="max">The maximum values for each component.</param>
    /// <returns>The clamped vector.</returns>
    public static Vector2 Clamp(Vector2 value, float min, float max)
    {
      float x = Clamp(value.x, min, max);
      float y = Clamp(value.y, min, max);
      return new Vector2(x, y);
    }

    /// <summary>
    /// Clamps the components of a 3D vector to the specified range.
    /// </summary>
    /// <param name="value">The vector to clamp.</param>
    /// <param name="min">The minimum values for each component.</param>
    /// <param name="max">The maximum values for each component.</param>
    /// <returns>The clamped vector.</returns>
    public static Vector3 Clamp(Vector3 value, Vector3 min, Vector3 max)
    {
      float x = Clamp(value.x, min.x, max.x);
      float y = Clamp(value.y, min.y, max.y);
      float z = Clamp(value.z, min.z, max.z);
      return new Vector3(x, y, z);
    }

    /// <summary>
    /// Clamps the components of a 3D vector to the specified range.
    /// </summary>
    /// <param name="value">The vector to clamp.</param>
    /// <param name="min">The minimum values for each component.</param>
    /// <param name="max">The maximum values for each component.</param>
    /// <returns>The clamped vector.</returns>
    public static Vector3 Clamp(Vector3 value, float min, float max)
    {
      float x = Clamp(value.x, min, max);
      float y = Clamp(value.y, min, max);
      float z = Clamp(value.z, min, max);
      return new Vector3(x, y, z);
    }

    /// <summary>
    /// Returns the sine of the specified angle in radians.
    /// </summary>
    /// <param name="angle">The angle in radians.</param>
    /// <returns>The sine of the angle.</returns>
    public static float Sin(float angle)
    {
      return (float)Math.Sin(angle);
    }

    /// <summary>
    /// Returns the arcsine of the specified value, in the range of -pi/2 to pi/2 radians.
    /// </summary>
    /// <param name="value">The value whose arcsine is to be calculated.</param>
    /// <returns>The arcsine of the specified value.</returns>
    public static float Asin(float value)
    {
      return (float)Math.Asin(value);
    }

    /// <summary>
    /// Returns the cosine of the specified angle in radians.
    /// </summary>
    /// <param name="angle">The angle in radians.</param>
    /// <returns>The cosine of the angle.</returns>
    public static float Cos(float angle)
    {
      return (float)Math.Cos(angle);
    }

    /// <summary>
    /// Returns the arccosine of the specified value, in the range of 0 to pi radians.
    /// </summary>
    /// <param name="value">The value whose arccosine is to be calculated.</param>
    /// <returns>The arccosine of the specified value.</returns>
    public static float Acos(float value)
    {
      return (float)Math.Acos(value);
    }

    /// <summary>
    /// Returns the tangent of the specified angle in radians.
    /// </summary>
    /// <param name="angle">The angle in radians.</param>
    /// <returns>The tangent of the angle.</returns>
    public static float Tan(float angle)
    {
      return (float)Math.Tan(angle);
    }

    /// <summary>
    /// Returns the arctangent of the specified value, in the range of -pi/2 to pi/2 radians.
    /// </summary>
    /// <param name="value">The value whose arctangent is to be calculated.</param>
    /// <returns>The arctangent of the specified value.</returns>
    public static float Atan(float value)
    {
      return (float)Math.Atan(value);
    }

    /// <summary>
    /// Returns the angle whose tangent is the quotient of the specified y-coordinate and x-coordinate.
    /// </summary>
    /// <param name="y">The y-coordinate of a point.</param>
    /// <param name="x">The x-coordinate of a point.</param>
    /// <returns>The angle whose tangent is the quotient of the specified y-coordinate and x-coordinate.</returns>
    public static float Atan2(float y, float x)
    {
      return (float)Math.Atan2(y, x);
    }

    /// <summary>
    /// Takes the square root of the value.
    /// </summary>
    /// <param name="value">Value to take square root.</param>
    /// <returns>Square root of the value.</returns>
    public static float Sqrt(float value)
    {
      return (float)Math.Sqrt(value);
    }

    /// <summary>
    /// Takes <c>power</c>'th power of the value.
    /// </summary>
    /// <param name="value">Value to take power.</param>
    /// <param name="power">Power value</param>
    /// <returns>power to the power of value</returns>
    public static float Pow(float value, float power)
    {
      return (float)Math.Pow(value, power);
    }
  }
}

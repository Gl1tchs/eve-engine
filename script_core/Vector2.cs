using System;

namespace EveEngine
{
  /// <summary>
  /// Represents a 2D vector with X and Y components.
  /// </summary>
  public struct Vector2
  {
    /// <summary>
    /// Gets or sets the X component of the vector.
    /// </summary>
    public float x;

    /// <summary>
    /// Gets or sets the Y component of the vector.
    /// </summary>
    public float y;

#pragma warning disable CS1591
    public static Vector2 Zero => new(0.0f);
    public static Vector2 One => new(1.0f);
    public static Vector2 Up => new(0.0f, 1.0f);
    public static Vector2 Down => new(0.0f, -1.0f);
    public static Vector2 Right => new(1.0f, 0.0f);
    public static Vector2 Left => new(-1.0f, 0.0f);
#pragma warning restore CS1591

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector2"/> struct with both components set to a specified value.
    /// </summary>
    /// <param name="scalar">The value to set both components to.</param>
    public Vector2(float scalar)
    {
      x = scalar;
      y = scalar;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector2"/> struct with specified X and Y components.
    /// </summary>
    /// <param name="x">The X component of the vector.</param>
    /// <param name="y">The Y component of the vector.</param>
    public Vector2(float x, float y)
    {
      this.x = x;
      this.y = y;
    }

    /// <summary>
    /// Calculates the squared length of the vector.
    /// </summary>
    /// <returns>The squared length of the vector.</returns>
    public readonly float LengthSquared()
    {
      return x * x + y * y;
    }

    /// <summary>
    /// Calculates the length of the vector.
    /// </summary>
    /// <returns>The length of the vector.</returns>
    public readonly float Length()
    {
      return (float)Math.Sqrt(LengthSquared());
    }

    /// <summary>
    /// Adds two vectors component-wise.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The result of adding the vectors.</returns>
    public static Vector2 operator +(Vector2 a, Vector2 b)
    {
      return new Vector2(a.x + b.x, a.y + b.y);
    }

    /// <summary>
    /// Subtracts the components of one vector from another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The result of subtracting the vectors.</returns>
    public static Vector2 operator -(Vector2 a, Vector2 b)
    {
      return new Vector2(a.x - b.x, a.y - b.y);
    }

    /// <summary>
    /// Multiplies a vector by a scalar value.
    /// </summary>
    /// <param name="vector">The vector to multiply.</param>
    /// <param name="scalar">The scalar value to multiply by.</param>
    /// <returns>The result of the multiplication.</returns>
    public static Vector2 operator *(Vector2 vector, float scalar)
    {
      return new Vector2(vector.x * scalar, vector.y * scalar);
    }

    /// <summary>
    /// Divides a vector by a scalar value.
    /// </summary>
    /// <param name="vector">The vector to divide.</param>
    /// <param name="divisor">The scalar value to divide by.</param>
    /// <returns>The result of the division.</returns>
    public static Vector2 operator /(Vector2 vector, float divisor)
    {
      // Check if divisor is not zero to avoid division by zero
      if (divisor != 0)
        return new Vector2(vector.x / divisor, vector.y / divisor);
      else
        throw new ArgumentException("Division by zero is not allowed.");
    }

    /// <summary>
    /// Determines whether two vectors are equal.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the vectors are equal; otherwise, <c>false</c>.</returns>
    public static bool operator ==(Vector2 a, Vector2 b)
    {
      return a.x == b.x && a.y == b.y;
    }

    /// <summary>
    /// Determines whether two vectors are not equal.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the vectors are not equal; otherwise, <c>false</c>.</returns>
    public static bool operator !=(Vector2 a, Vector2 b)
    {
      return !(a == b);
    }

    /// <summary>
    /// Determines whether one vector is greater than another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the first vector is greater than the second vector; otherwise, <c>false</c>.</returns>
    public static bool operator >(Vector2 a, Vector2 b)
    {
      return a.x > b.x && a.y > b.y;
    }

    /// <summary>
    /// Determines whether one vector is less than another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the first vector is less than the second vector; otherwise, <c>false</c>.</returns>
    public static bool operator <(Vector2 a, Vector2 b)
    {
      return a.x < b.x && a.y < b.y;
    }

    /// <summary>
    /// Determines whether the current vector is equal to another vector.
    /// </summary>
    /// <param name="obj">The object to compare with the current vector.</param>
    /// <returns><c>true</c> if the objects are equal; otherwise, <c>false</c>.</returns>
    public override bool Equals(object obj)
    {
      if (obj is Vector2 other)
      {
        return this == other;
      }
      return false;
    }

    /// <summary>
    /// Gets a hash code for the vector.
    /// </summary>
    /// <returns>A hash code for the vector.</returns>
    public override int GetHashCode()
    {
      return x.GetHashCode() ^ y.GetHashCode();
    }

    /// <summary>
    /// Returns a string representation of the vector.
    /// </summary>
    /// <returns>A string representation of the vector.</returns>
    public override readonly string ToString()
    {
      return String.Format("Vector2({0}, {1})", x, y);
    }
  }
}
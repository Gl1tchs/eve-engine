using System;

namespace EveEngine
{
  /// <summary>
  /// Represents a 3D vector with X, Y, and Z components.
  /// </summary>
  public struct Vector3
  {
    /// <summary>
    /// Gets or sets the X component of the vector.
    /// </summary>
    public float x;

    /// <summary>
    /// Gets or sets the Y component of the vector.
    /// </summary>
    public float y;

    /// <summary>
    /// Gets or sets the Z component of the vector.
    /// </summary>
    public float z;

#pragma warning disable CS1591
    public static Vector3 Zero => new(0.0f);
    public static Vector3 One => new(1.0f);
    public static Vector3 Front => new(0.0f, 0.0f, -1.0f);
    public static Vector3 Back => new(0.0f, 0.0f, 1.0f);
    public static Vector3 Up => new(0.0f, 1.0f, 0.0f);
    public static Vector3 Down => new(0.0f, -1.0f, 0.0f);
    public static Vector3 Left => new(-1.0f, 0.0f, 0.0f);
#pragma warning restore CS1591

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector3"/> struct with all components set to a specified value.
    /// </summary>
    /// <param name="scalar">The value to set all components to.</param>
    public Vector3(float scalar)
    {
      x = scalar;
      y = scalar;
      z = scalar;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector3"/> struct with specified X, Y, and Z components.
    /// </summary>
    /// <param name="x">The X component of the vector.</param>
    /// <param name="y">The Y component of the vector.</param>
    /// <param name="z">The Z component of the vector.</param>
    public Vector3(float x, float y, float z)
    {
      this.x = x;
      this.y = y;
      this.z = z;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector3"/> struct with specified XY components and Z component.
    /// </summary>
    /// <param name="xy">The XY components of the vector.</param>
    /// <param name="z">The Z component of the vector.</param>
    public Vector3(Vector2 xy, float z)
    {
      x = xy.x;
      y = xy.y;
      this.z = z;
    }

    /// <summary>
    /// Gets or sets the XY components of the vector.
    /// </summary>
    public Vector2 XY
    {
      readonly get => new(x, y);
      set
      {
        x = value.x;
        y = value.y;
      }
    }

    /// <summary>
    /// Calculates the length of the vector.
    /// </summary>
    /// <returns>The length of the vector.</returns>
    public readonly float Length()
    {
      return (float)Math.Sqrt(x * x + y * y + z * z);
    }

    /// <summary>
    /// Adds two vectors component-wise.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The result of adding the vectors.</returns>
    public static Vector3 operator +(Vector3 a, Vector3 b)
    {
      return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    /// <summary>
    /// Subtracts the components of one vector from another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The result of subtracting the vectors.</returns>
    public static Vector3 operator -(Vector3 a, Vector3 b)
    {
      return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    /// <summary>
    /// Multiplies a vector by a scalar value.
    /// </summary>
    /// <param name="vector">The vector to multiply.</param>
    /// <param name="scalar">The scalar value to multiply by.</param>
    /// <returns>The result of the multiplication.</returns>
    public static Vector3 operator *(Vector3 vector, float scalar)
    {
      return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    }

    /// <summary>
    /// Divides a vector by a scalar value.
    /// </summary>
    /// <param name="vector">The vector to divide.</param>
    /// <param name="divisor">The scalar value to divide by.</param>
    /// <returns>The result of the division.</returns>
    /// <exception cref="ArgumentException">Thrown when division by zero is attempted.</exception>
    public static Vector3 operator /(Vector3 vector, float divisor)
    {
      // Check if divisor is not zero to avoid division by zero
      if (divisor != 0)
        return new Vector3(vector.x / divisor, vector.y / divisor, vector.z / divisor);
      else
        throw new ArgumentException("Division by zero is not allowed.");
    }

    /// <summary>
    /// Determines whether two vectors are equal.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the vectors are equal; otherwise, <c>false</c>.</returns>
    public static bool operator ==(Vector3 a, Vector3 b)
    {
      return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    /// <summary>
    /// Determines whether two vectors are not equal.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the vectors are not equal; otherwise, <c>false</c>.</returns>
    public static bool operator !=(Vector3 a, Vector3 b)
    {
      return !(a == b);
    }

    /// <summary>
    /// Determines whether one vector is greater than another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the first vector is greater than the second vector; otherwise, <c>false</c>.</returns>
    public static bool operator >(Vector3 a, Vector3 b)
    {
      return a.x > b.x && a.y > b.y && a.z > b.z;
    }

    /// <summary>
    /// Determines whether one vector is less than another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the first vector is less than the second vector; otherwise, <c>false</c>.</returns>
    public static bool operator <(Vector3 a, Vector3 b)
    {
      return a.x < b.x && a.y < b.y && a.z < b.z;
    }

    /// <summary>
    /// Determines whether the current vector is equal to another vector.
    /// </summary>
    /// <param name="obj">The object to compare with the current vector.</param>
    /// <returns><c>true</c> if the objects are equal; otherwise, <c>false</c>.</returns>
    public override readonly bool Equals(object obj)
    {
      if (obj is Vector3 other)
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
      return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode();
    }

    /// <summary>
    /// Returns a string representation of the vector.
    /// </summary>
    /// <returns>A string representation of the vector.</returns>
    public override readonly string ToString()
    {
      return string.Format("Vector3({0}, {1}, {2})", x, y, z);
    }
  }
}

using System;

namespace EveEngine
{
  /// <summary>
  /// Represents a vector with X, Y, Z, and W components.
  /// </summary>
  public struct Vector4
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

    /// <summary>
    /// Gets or sets the W component of the vector.
    /// </summary>
    public float w;

#pragma warning disable CS1591
    public static Vector4 Zero => new(0.0f);
    public static Vector4 One => new(1.0f);
#pragma warning restore CS1591

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector4"/> struct with all components set to a specified value.
    /// </summary>
    /// <param name="scalar">The value to set all components to.</param>
    public Vector4(float scalar)
    {
      x = scalar;
      y = scalar;
      z = scalar;
      w = scalar;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector4"/> struct with specified X, Y, Z, and W components.
    /// </summary>
    /// <param name="x">The X component of the vector.</param>
    /// <param name="y">The Y component of the vector.</param>
    /// <param name="z">The Z component of the vector.</param>
    /// <param name="w">The W component of the vector.</param>
    public Vector4(float x, float y, float z, float w)
    {
      this.x = x;
      this.y = y;
      this.z = z;
      this.w = w;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="Vector4"/> struct with specified XYZ components and W component.
    /// </summary>
    /// <param name="xyz">The XYZ components of the vector.</param>
    /// <param name="w">The W component of the vector.</param>
    public Vector4(Vector3 xyz, float w)
    {
      x = xyz.x;
      y = xyz.y;
      z = xyz.z;
      this.w = w;
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
    /// Gets or sets the XYZ components of the vector.
    /// </summary>
    public Vector3 XYZ
    {
      readonly get => new(x, y, z);
      set
      {
        x = value.x;
        y = value.y;
        z = value.z;
      }
    }

    /// <summary>
    /// Adds two vectors component-wise.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The result of adding the vectors.</returns>
    public static Vector4 operator +(Vector4 a, Vector4 b)
    {
      return new Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    /// <summary>
    /// Subtracts the components of one vector from another.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns>The result of subtracting the vectors.</returns>
    public static Vector4 operator -(Vector4 a, Vector4 b)
    {
      return new Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    /// <summary>
    /// Multiplies a vector by a scalar value.
    /// </summary>
    /// <param name="vector">The vector to multiply.</param>
    /// <param name="scalar">The scalar value to multiply by.</param>
    /// <returns>The result of the multiplication.</returns>
    public static Vector4 operator *(Vector4 vector, float scalar)
    {
      return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
    }

    /// <summary>
    /// Divides a vector by a scalar value.
    /// </summary>
    /// <param name="vector">The vector to divide.</param>
    /// <param name="divisor">The scalar value to divide by.</param>
    /// <returns>The result of the division.</returns>
    /// <exception cref="ArgumentException">Thrown when division by zero is attempted.</exception>
    public static Vector4 operator /(Vector4 vector, float divisor)
    {
      // Check if divisor is not zero to avoid division by zero
      if (divisor != 0)
        return new Vector4(vector.x / divisor, vector.y / divisor, vector.z / divisor, vector.w / divisor);
      else
        throw new ArgumentException("Division by zero is not allowed.");
    }

    /// <summary>
    /// Determines whether two vectors are equal.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the vectors are equal; otherwise, <c>false</c>.</returns>
    public static bool operator ==(Vector4 a, Vector4 b)
    {
      return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    /// <summary>
    /// Determines whether two vectors are not equal.
    /// </summary>
    /// <param name="a">The first vector.</param>
    /// <param name="b">The second vector.</param>
    /// <returns><c>true</c> if the vectors are not equal; otherwise, <c>false</c>.</returns>
    public static bool operator !=(Vector4 a, Vector4 b)
    {
      return !(a == b);
    }

    /// <summary>
    /// Determines whether the current vector is equal to another vector.
    /// </summary>
    /// <param name="obj">The object to compare with the current vector.</param>
    /// <returns><c>true</c> if the objects are equal; otherwise, <c>false</c>.</returns>
    public override bool Equals(object obj)
    {
      if (obj is Vector4 other)
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
      return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode() ^ w.GetHashCode();
    }

    /// <summary>
    /// Returns a string representation of the vector.
    /// </summary>
    /// <returns>A string representation of the vector.</returns>
    public override string ToString()
    {
      return string.Format("Vector4({0}, {1}, {2}, {3})", x, y, z, w);
    }
  }
}

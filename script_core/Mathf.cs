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
  }
}

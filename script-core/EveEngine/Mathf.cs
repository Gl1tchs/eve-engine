using System;

namespace EveEngine
{
    public static class Mathf
    {
        public static float EPSILON = 1e-5f;

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

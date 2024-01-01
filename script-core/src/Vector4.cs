using System;

namespace EveEngine
{
    public struct Vector4
    {
        public float x, y, z, w;

        public static Vector4 Zero => new Vector4(0.0f);

        public Vector4(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
            w = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public Vector4(Vector3 xyz, float w)
        {
            x = xyz.x;
            y = xyz.y;
            z = xyz.z;
            this.w = w;
        }

        public Vector2 XY
        {
            get => new Vector2(x, y);
            set
            {
                x = value.x;
                y = value.y;
            }
        }

        public Vector3 XYZ
        {
            get => new Vector3(x, y, z);
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
            }
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
        }

        public static Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
        }

        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
        }

        public static Vector4 operator /(Vector4 vector, float divisor)
        {
            // Check if divisor is not zero to avoid division by zero
            if (divisor != 0)
                return new Vector4(vector.x / divisor, vector.y / divisor, vector.z / divisor, vector.w / divisor);
            else
                throw new ArgumentException("Division by zero is not allowed.");
        }

        public static bool operator ==(Vector4 a, Vector4 b)
        {
            return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
        }

        public static bool operator !=(Vector4 a, Vector4 b)
        {
            return !(a == b);
        }

        public override bool Equals(object obj)
        {
            if (obj is Vector4 other)
            {
                return this == other;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode() ^ w.GetHashCode();
        }

        public override string ToString()
        {
            return String.Format("Vector4({0}, {1}, {2}, {3})", x, y, z, w);
        }
    }
}

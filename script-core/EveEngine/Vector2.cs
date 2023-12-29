using System;

namespace EveEngine
{
	public struct Vector2
	{
		public float x, y;

		public static Vector2 Zero => new Vector2(0.0f);

		public Vector2(float scalar)
		{
			x = scalar;
			y = scalar;
		}

		public Vector2(float x, float y)
		{
			this.x = x;
			this.y = y;
		}

		public float LengthSquared()
		{
			return x * x + y * y;
		}

		public float Length()
		{
			return (float)Math.Sqrt(LengthSquared());
		}

		public static Vector2 operator +(Vector2 a, Vector2 b)
		{
			return new Vector2(a.x + b.x, a.y + b.y);
		}

		public static Vector2 operator -(Vector2 a, Vector2 b)
		{
			return new Vector2(a.x - b.x, a.y - b.y);
		}

		public static Vector2 operator *(Vector2 vector, float scalar)
		{
			return new Vector2(vector.x * scalar, vector.y * scalar);
		}

		public static Vector2 operator /(Vector2 vector, float divisor)
		{
			// Check if divisor is not zero to avoid division by zero
			if (divisor != 0)
				return new Vector2(vector.x / divisor, vector.y / divisor);
			else
				throw new ArgumentException("Division by zero is not allowed.");
		}

		public static bool operator ==(Vector2 a, Vector2 b)
		{
			return a.x == b.x && a.y == b.y;
		}

		public static bool operator !=(Vector2 a, Vector2 b)
		{
			return !(a == b);
		}

		public static bool operator >(Vector2 a, Vector2 b)
		{
			return a.x > b.x && a.y > b.y;
		}

		public static bool operator <(Vector2 a, Vector2 b)
		{
			return a.x < b.x && a.y < b.y;
		}

		public override bool Equals(object obj)
		{
			if (obj is Vector2 other)
			{
				return this == other;
			}
			return false;
		}

		public override int GetHashCode()
		{
			return x.GetHashCode() ^ y.GetHashCode();
		}

		public override string ToString()
		{
			return String.Format("Vector2({0}, {1})", x, y);
		}
	}
}

﻿using System;

namespace EveEngine
{
	public struct Vector3
	{
		public float x, y, z;

		public static Vector3 Zero => new Vector3(0.0f);

		public Vector3(float scalar)
		{
			x = scalar;
			y = scalar;
			z = scalar;
		}

		public Vector3(float x, float y, float z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}

		public Vector3(Vector2 xy, float z)
		{
			x = xy.x;
			y = xy.y;
			this.z = z;
		}

		public Vector2 xy
		{
			get => new Vector2(x, y);
			set
			{
				x = value.x;
				y = value.y;
			}
		}

		public float Length()
		{
			return (float)Math.Sqrt(x * x + y * y + z * z);
		}

		public static Vector3 operator +(Vector3 a, Vector3 b)
		{
			return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		public static Vector3 operator -(Vector3 a, Vector3 b)
		{
			return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		public static Vector3 operator *(Vector3 vector, float scalar)
		{
			return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		public static Vector3 operator /(Vector3 vector, float divisor)
		{
			// Check if divisor is not zero to avoid division by zero
			if (divisor != 0)
				return new Vector3(vector.x / divisor, vector.y / divisor, vector.z / divisor);
			else
				throw new ArgumentException("Division by zero is not allowed.");
		}

		public static bool operator ==(Vector3 a, Vector3 b)
		{
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}

		public static bool operator !=(Vector3 a, Vector3 b)
		{
			return !(a == b);
		}

		public static bool operator >(Vector3 a, Vector3 b)
		{
			return a.x > b.x && a.y > b.y && a.z > b.z;
		}

		public static bool operator <(Vector3 a, Vector3 b)
		{
			return a.x < b.x && a.y < b.y && a.z < b.z;
		}

		public override bool Equals(object obj)
		{
			if (obj is Vector3)
			{
				Vector3 other = (Vector3)obj;
				return this == other;
			}
			return false;
		}

		public override int GetHashCode()
		{
			return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode();
		}
	}
}
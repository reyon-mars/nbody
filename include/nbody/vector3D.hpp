#pragma once

#include <cmath>
#include <ostream>

namespace nbody
{
	/*
 * The struct Represents a 3D vector, such as a position, velocity, acceleration etc.
 */
	struct Vector3D
	{
		double x{0.0};
		double y{0.0};
		double z{0.0};

		constexpr Vector3D() noexcept = default;
		constexpr Vector3D(double x_, double y_, double z_) noexcept : x(x_), y(y_), z(z_)
		{
		}

		constexpr Vector3D& operator+=(const Vector3D& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		constexpr Vector3D& operator-=(const Vector3D& rhs) noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		constexpr Vector3D& operator*=(const double s) noexcept
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}
		constexpr Vector3D& operator/=(const double s) noexcept
		{
			x /= s;
			y /= s;
			z /= s;
			return *this;
		}

		[[nodiscard]] constexpr double lengthSquared() const noexcept
		{
			return (x * x) + (y * y) + (z * z);
		}

		[[nodiscard]] constexpr double length() const noexcept
		{
			return std::sqrt(lengthSquared());
		}

		[[nodiscard]] constexpr Vector3D normalized() const noexcept
		{
			const double len = length();
			return Vector3D{x / len, y / len, z / len};
		}
	};

	[[nodiscard]] constexpr Vector3D operator+(Vector3D a, const Vector3D& b) noexcept
	{
		a += b;
		return a;
	}

	[[nodiscard]] constexpr Vector3D operator-(Vector3D a, const Vector3D& b) noexcept
	{
		a -= b;
		return a;
	}

	[[nodiscard]] constexpr Vector3D operator-(Vector3D a) noexcept
	{
		return Vector3D{-a.x, -a.y, -a.z};
	}

	[[nodiscard]] constexpr Vector3D operator*(Vector3D a, double s) noexcept
	{
		a *= s;
		return a;
	}

	[[nodiscard]] constexpr Vector3D operator*(double s, Vector3D a) noexcept
	{
		a *= s;
		return a;
	}

	[[nodiscard]] constexpr Vector3D operator/(Vector3D a, double s) noexcept
	{
		a /= s;
		return a;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector3D& v)
	{
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << ")";
	}
} // namespace nbody
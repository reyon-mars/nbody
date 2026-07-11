#pragma once

#include <cmath>
#include <ostream>

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


    constexpr
};
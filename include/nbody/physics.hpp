#pragma once

#include "./body.hpp"
#include "./vector3D.hpp"
#include <cstddef>
#include <vector>

namespace nbody
{
	/*
     * Newton's Law of Universal Gravitation (Vector Formulation)
     * Computes the gravitational force vector exerted ON body_i BY body_j
     *
     * Mathematical Form:
     *                          m_i * m_j
     *          F_ij = G * __________________ * u_ij
     *                     || r_j - r_i || ^ 2
     */
	[[nodiscard]] inline Vector3D gravitationalForce(const Body& on, const Body& from, double G) noexcept
	{
		const Vector3D displacement = on.position - from.position;
		const double distance = displacement.length();
		const Vector3D direction = displacement.normalized();
		const double F_ij = G * (on.mass * from.mass) / (distance * distance);
		return -F_ij * direction;
	}

	/*
     * Computes the total net gravitational force acting on a specific body. 
     */
	[[nodiscard]] inline Vector3D netForce(const std::vector<Body>& bodies, std::size_t i, double G) noexcept
	{
		Vector3D total{};
		const Body& target = bodies[i];
		const std::size_t size = bodies.size();

		for (std::size_t j = 0; j < size; ++j)
		{
			if (j == i) [[unlikely]]
			{
				continue;
			}
			total += gravitationalForce(target, bodies[j], G);
		}
		return total;
	}

} // namespace nbody
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
     *          F_ij = -G * __________________ * u_ij
     *                     || r_i - r_j || ^ 2
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
     * Computes the total net gravitational force acting on a specific body
     * by summing up the individual force vectors from all other bodies.
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

	/*
     * Compute and fill in the acceleration for every body using Newton's Second Law.
     * This runs as a standalone first pass to gather a single, consistent snapshot
     * of forces across the system before any positions or velocities are altered.
     *
     * F = m * a
     * a = F / m
     */
	inline void computeAcceleration(std::vector<Body>& bodies, double G) noexcept
	{
		std::size_t size = bodies.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			bodies[i].acceleration = netForce(bodies, i, G) / bodies[i].mass;
		}
	}

	/*
     * The Euler Integrator.
     * Walks down the physics dependency chain: uses the static acceleration snapshot
     * to update the velocity, and then uses that updated velocity to advance the 
     * position forward by the time step dt.
     *
     * velocity += acceleration * dt
     * position += velocity * dt
     */
	inline void EulerStep(std::vector<Body>& bodies, double dt, double G) noexcept
	{
		computeAcceleration(bodies, G);
		for (auto& body : bodies)
		{
			body.velocity += body.acceleration * dt;
			body.position += body.velocity * dt;
		}
	}

} // namespace nbody
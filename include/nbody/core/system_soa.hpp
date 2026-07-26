#pragma once
#include "nbody/core/vector3D.hpp"
#include "nbody/physics/body.hpp"
#include <cmath>
#include <cstddef>
#include <vector>

namespace nbody
{
	struct BodySystemSoA
	{
		alignas(64) std::vector<double> x, y, z;
		alignas(64) std::vector<double> vx, vy, vz;
		alignas(64) std::vector<double> ax, ay, az;
		alignas(64) std::vector<double> mass;

		explicit BodySystemSoA(const std::vector<Body>& bodies)
		{
			const std::size_t n = bodies.size();
			resize(n);

			for (std::size_t i = 0; i < n; ++i)
			{
				x[i] = bodies[i].position.x;
				y[i] = bodies[i].position.y;
				z[i] = bodies[i].position.z;
				vx[i] = bodies[i].velocity.x;
				vy[i] = bodies[i].velocity.y;
				vz[i] = bodies[i].velocity.z;
				mass[i] = bodies[i].mass;
			}
		}

		[[nodiscard]] std::vector<Body> toAoS() const
		{
			const std::size_t n = size();
			std::vector<Body> bodies(n);

			for (std::size_t i = 0; i < n; ++i)
			{
				bodies[i].position = Vector3D{x[i], y[i], z[i]};
				bodies[i].velocity = Vector3D{vx[i], vy[i], vz[i]};
				bodies[i].acceleration = Vector3D{ax[i], ay[i], az[i]};
				bodies[i].mass = mass[i];
			}
			return bodies;
		}

		[[nodiscard]] std::size_t size() const noexcept
		{
			return x.size();
		}

		void resize(std::size_t n)
		{
			x.resize(n, 0.0);
			y.resize(n, 0.0);
			z.resize(n, 0.0);
			vx.resize(n, 0.0);
			vy.resize(n, 0.0);
			vz.resize(n, 0.0);
			ax.resize(n, 0.0);
			ay.resize(n, 0.0);
			az.resize(n, 0.0);
			mass.resize(n, 1.0);
		}

		void reserve(std::size_t n)
		{
			x.reserve(n);
			y.reserve(n);
			z.reserve(n);
			vx.reserve(n);
			vy.reserve(n);
			vz.reserve(n);
			ax.reserve(n);
			ay.reserve(n);
			az.reserve(n);
			mass.reserve(n);
		}
	};
} // namespace nbody
#pragma once

#include "nbody/core/system_soa.hpp"
#include <cmath>
#include <cstddef>

namespace nbody::simd::autovec
{
	inline void computeAccelerations(BodySystemSoA& soa, double G)
	{
		const std::size_t n = soa.size();
		const double* __restrict px = soa.x.data();
		const double* __restrict py = soa.y.data();
		const double* __restrict pz = soa.z.data();
		const double* __restrict pm = soa.mass.data();

		for (std::size_t i = 0; i < n; ++i)
		{
			/*
             * { xi, yi, zi } the position vector of the body at index i.
             */
			const double xi = px[i];
			const double yi = py[i];
			const double zi = pz[i];

			double fx{0.0};
			double fy{0.0};
			double fz{0.0};

#pragma omp simd reduction(+ : fx, fy, fz)
			for (std::size_t j = 0; j < n; ++j)
			{
				/*
                 * { dx, dy, dz } the displacement vector between the body at 
                 * index i and at index j.
                 */
				const double dx = xi - px[j];
				const double dy = yi - py[j];
				const double dz = zi - pz[j];

				const double distSquared = (dx * dx) + (dy * dy) + (dz * dz) + kSofteningSquared;
				const double dist = std::sqrt(distSquared);
				const double invDistCubed = 1.0 / distSquared * dist;

				const double acceleration = -G * pm[j] * invDistCubed;

				fx += acceleration * dx;
				fy += acceleration * dy;
				fz += acceleration * dz;
			}
			soa.ax[i] = fx;
			soa.ay[i] = fy;
			soa.az[i] = fz;
		}
	}
} // namespace nbody::simd::autovec
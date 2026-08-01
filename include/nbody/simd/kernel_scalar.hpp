#pragma once

#include "nbody/core/system_soa.hpp"
#include <cmath>
#include <cstddef>

namespace nbody::simd::scalar
{
	inline void computeAccelerations(BodySystemSoA& soa, double G)
	{
		const std::size_t n = soa.size();
		for (std::size_t i = 0; i < n; ++i)
		{
			double sumX{0.0}, sumY{0.0}, sumZ{0.0};
			for (std::size_t j = 0; j < n; ++j)
			{
				const double dX = soa.x[i] - soa.x[j];
				const double dY = soa.y[i] - soa.y[j];
				const double dZ = soa.z[i] - soa.z[j];
				const double distSq = dX * dX + dY * dY + dZ * dZ + kSofteningSquared;
				const double dist = std::sqrt(distSq);
				const double acceleration = -G * soa.mass[j] / (dist * distSq);
				sumX += acceleration * dX;
				sumY += acceleration * dY;
				sumZ += acceleration * dZ;
			}
			soa.ax[i] = sumX;
			soa.ay[i] = sumY;
			soa.az[i] = sumZ;
		}
	}
} // namespace nbody::simd::scalar
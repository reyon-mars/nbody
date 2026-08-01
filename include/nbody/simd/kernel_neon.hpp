#pragma once

#if defined(__ARM_NEON__) || defined(__aarch64__)

#include "nbody/core/system_soa.hpp"
#include <arm_neon.h>
#include <cmath>
#include <cstddef>

namespace nbody::simd::neon
{
	inline void computeAccelerations(BodySystemSoA& soa, double G)
	{
		const std::size_t n = soa.size();
		const double* px = soa.x.data();
		const double* py = soa.y.data();
		const double* pz = soa.z.data();
		const double* pm = soa.mass.data();

		const float64x2_t soft = vdupq_n_f64(kSofteningSquared);

		for (std::size_t i = 0; i < n; ++i)
		{
			const float64x2_t xi = vdupq_n_f64(px[i]);
			const float64x2_t yi = vdupq_n_f64(py[i]);
			const float64x2_t zi = vdupq_n_f64(pz[i]);

			const float64x2_t negG = vdupq_n_f64(-G);

			float64x2_t fx = vdupq_n_f64(0.0);
			float64x2_t fy = vdupq_n_f64(0.0);
			float64x2_t fz = vdupq_n_f64(0.0);

			std::size_t j{0};
			for (; (j + 2) < n; j += 2)
			{
				const float64x2_t xj = vld1q_f64(&px[j]);
				const float64x2_t yj = vld1q_f64(&py[j]);
				const float64x2_t zj = vld1q_f64(&pz[j]);
				const float64x2_t mj = vld1q_f64(&pm[j]);

				const float64x2_t dx = vsubq_f64(xi, xj);
				const float64x2_t dy = vsubq_f64(yi, yj);
				const float64x2_t dz = vsubq_f64(zi, zj);

				float64x2_t distSquared = vmulq_f64(dx, dx);
				distSquared = vaddq_f64(distSquared, vmulq_f64(dy, dy));
				distSquared = vaddq_f64(distSquared, vmulq_f64(dz, dz));
				distSquared = vaddq_f64(distSquared, soft);

				const float64x2_t dist = vsqrtq_f64(distSquared);
				const float64x2_t distCubed = vmulq_f64(dist, distSquared);
				const float64x2_t acceleration = vdivq_f64(vmulq_f64(negG, mj), distCubed);
				fx = vaddq_f64(fx, vmulq_f64(acceleration, dx));
				fy = vaddq_f64(fy, vmulq_f64(acceleration, dy));
				fz = vaddq_f64(fz, vmulq_f64(acceleration, dz));
			}

			double sumX = vgetq_lane_f64(fx, 0) + vgetq_lane_f64(fx, 1);
			double sumY = vgetq_lane_f64(fy, 0) + vgetq_lane_f64(fy, 1);
			double sumZ = vgetq_lane_f64(fz, 0) + vgetq_lane_f64(fz, 1);

			for (; j < n; ++j)
			{
				const double dx = px[i] - px[j];
				const double dy = py[i] - py[j];
				const double dz = pz[i] - pz[j];
				const double distSquared = (dx * dx) + (dy * dy) + (dz * dz) + kSofteningSquared;
				const double dist = std::sqrt(distSquared);
				const double invDistCubed = 1.0 / (dist * distSquared);
				const double acceleration = -G * pm[j] * invDistCubed;
				sumX += acceleration * dx;
				sumY += acceleration * dy;
				sumZ += acceleration * dz;
			}
			soa.ax[i] = sumX;
			soa.ay[i] = sumY;
			soa.az[i] = sumZ;
		}
	}
} // namespace nbody::simd::neon

#endif
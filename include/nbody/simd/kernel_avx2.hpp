#pragma once

#if defined(__AVX2__)

#include "nbody/core/system_soa.hpp"
#include <cmath>
#include <cstddef>
#include <immintrin.h>

namespace nbody::simd::avx2
{
	inline void computeAccelerations(BodySystemSoA& soa, double G)
	{
		const std::size_t n = soa.size();
		const double* px = soa.x.data();
		const double* py = soa.y.data();
		const double* pz = soa.z.data();
		const double* pm = soa.mass.data();

		const __m256d soft = _mm256_set1_pd(kSofteningSquared);

		for (std::size_t i = 0; i < n; ++i)
		{
			const __m256d xi = _mm256_set1_pd(px[i]);
			const __m256d yi = _mm256_set1_pd(py[i]);
			const __m256d zi = _mm256_set1_pd(pz[i]);
			const __m256d negG = _mm256_set1_pd(-G);

			__m256d fx = _mm256_setzero_pd();
			__m256d fy = _mm256_setzero_pd();
			__m256d fz = _mm256_setzero_pd();

			std::size_t j{0};
			for (; (j + 4) <= n; j += 4)
			{
				const __m256d xj = _mm256_loadu_pd(px + j);
				const __m256d yj = _mm256_loadu_pd(py + j);
				const __m256d zj = _mm256_loadu_pd(pz + j);
				const __m256d mj = _mm256_loadu_pd(pm + j);

				const __m256d dx = _mm256_sub_pd(xi, xj);
				const __m256d dy = _mm256_sub_pd(yi, yj);
				const __m256d dz = _mm256_sub_pd(zi, zj);

				__m256d distSquared = _mm256_mul_pd(dx, dx);
				distSquared = _mm256_add_pd(distSquared, _mm256_mul_pd(dy, dy));
				distSquared = _mm256_add_pd(distSquared, _mm256_mul_pd(dz, dz));
				distSquared = _mm256_add_pd(distSquared, soft);

				const __m256d dist = _mm256_sqrt_pd(distSquared);
				const __m256d distCubed = _mm256_mul_pd(dist, distSquared);
				const __m256d acceleration = _mm256_div_pd(_mm256_mul_pd(negG, mj), distCubed);

				fx = _mm256_add_pd(fx, _mm256_mul_pd(acceleration, dx));
				fy = _mm256_add_pd(fy, _mm256_mul_pd(acceleration, dy));
				fz = _mm256_add_pd(fz, _mm256_mul_pd(acceleration, dz));
			}

			alignas(32) std::array<double, 4> fxArr, fyArr, fzArr;
			_mm256_storeu_pd(fxArr.data(), fx);
			_mm256_storeu_pd(fyArr.data(), fy);
			_mm256_storeu_pd(fzArr.data(), fz);

			double sumX = fxArr[0] + fxArr[1] + fxArr[2] + fxArr[3];
			double sumY = fyArr[0] + fyArr[1] + fyArr[2] + fyArr[3];
			double sumZ = fzArr[0] + fzArr[1] + fzArr[2] + fzArr[3];

			for (; j < n; ++j)
			{
				const double dx = px[i] - px[j];
				const double dy = py[i] - py[j];
				const double dz = pz[i] - pz[j];
				const double distSquared = (dx * dx) + (dy * dy) + (dz * dz) + kSofteningSquared;
				const double dist = std::sqrt(distSquared);
				const double invDistCubed = 1.0 / (distSquared * dist);
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
} // namespace nbody::simd::avx2

#endif
#pragma once

#include "nbody/core/system_soa.hpp"
#include "nbody/simd/kernel_auto_vec.hpp"
#include "nbody/simd/kernel_avx2.hpp"
#include "nbody/simd/kernel_neon.hpp"
#include "nbody/simd/kernel_scalar.hpp"

#if defined(__AVX2__)
#define NBODY_SIMD_BACKEND "AVX2 (x86, 4 doubles per register)"
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define NBODY_SIMD_BACKEND "NEON (ARM64, 2 doubles per register)"
#else
#define NBODY_SIMD_BACKEND "Scalar fallback (no SIMD ISA detected)"
#endif

namespace nbody::simd
{
	inline void computeAccelerationsSIMD(BodySystemSoA& soa, double G)
	{
#if defined(__AVX2__)
		avx2::computeAccelerations(soa, G);

#elif defined(__ARM_NEON) || defined(__aarch64__)
		neon::computeAccelerations(soa, G);

#else
		scalar::computeAccelerations(soa, G);
#endif
	}
} // namespace nbody::simd
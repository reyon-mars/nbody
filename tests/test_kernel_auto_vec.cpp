#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nbody/core/system_soa.hpp"
#include "nbody/physics/body.hpp"
#include "nbody/physics/physics.hpp"
#include "nbody/simd/kernel_auto_vec.hpp"
#include <random>
#include <vector>

#define AUTOVEC_TAG "[AUTOVEC]"

using nbody::Body;

namespace
{
	std::vector<Body> randomBodies(std::size_t size, unsigned seed)
	{
		std::mt19937 rng(seed);
		std::uniform_real_distribution<double> pos(-100.0, 100.0);
		std::uniform_real_distribution<double> mass(1.0, 10.0);
		std::vector<Body> bodies;
		bodies.reserve(size);

		for (std::size_t i = 0; i < size; ++i)
		{
			bodies.push_back(Body{.position = {pos(rng), pos(rng), pos(rng)}, .mass = mass(rng)});
		}
		return bodies;
	}
} // namespace

TEST_CASE("simd::autovec::computeAccelerations matches the AoS reference closely", AUTOVEC_TAG)
{
	const auto bodies = randomBodies(300, 22);
	const double G{1.0};

	auto reference = bodies;
	nbody::computeAcceleration(reference, G);

	nbody::BodySystemSoA soa(bodies);
	nbody::simd::autovec::computeAccelerations(soa, G);

	for (std::size_t i = 0; i < bodies.size(); ++i)
	{
		CHECK(soa.ax[i] == Catch::Approx(reference[i].acceleration.x).epsilon(1e-9));
		CHECK(soa.ay[i] == Catch::Approx(reference[i].acceleration.y).epsilon(1e-9));
		CHECK(soa.az[i] == Catch::Approx(reference[i].acceleration.z).epsilon(1e-9));
	}
}

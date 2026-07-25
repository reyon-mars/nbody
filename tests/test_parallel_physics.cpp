#include "catch2/catch_test_macros.hpp"
#include "nbody/body.hpp"
#include "nbody/parallel_physics.hpp"
#include <random>
#include <vector>

#define PARALLEL_PHYSICS "[parallel_physics]"

using nbody::Body;

namespace
{

	std::vector<Body> randomBodies(std::size_t n, unsigned seed)
	{
		std::mt19937 rng(seed);
		std::uniform_real_distribution<double> pos(-100.0, 100.0);
		std::uniform_real_distribution<double> mass(1.0, 10.0);

		std::vector<Body> bodies;
		bodies.reserve(n);

		for (std::size_t i = 0; i < n; ++i)
		{
			bodies.push_back(Body{.position = {pos(rng), pos(rng), pos(rng)}, .mass = mass(rng)});
		}
		return bodies;
	}
} // namespace

TEST_CASE("parallel acceleration computation is bit-for-bit identical to serial", PARALLEL_PHYSICS)
{
	const auto baseline = randomBodies(47, 1234);
	constexpr double G = 1.0;

	auto serialBodies = baseline;

	nbody::computeAcceleration(serialBodies, G);

	for (std::size_t threads : {std::size_t{1}, std::size_t{2}, std::size_t{4}, std::size_t{8}})
	{
		UNSCOPED_INFO("Testing thread count: " << threads);
		auto parallelBodies = baseline;
		nbody::computeAccelerationParallel(parallelBodies, G, threads);

		for (std::size_t i = 0; i < baseline.size(); ++i)
		{
			CHECK(parallelBodies[i].acceleration.x == serialBodies[i].acceleration.x);
			CHECK(parallelBodies[i].acceleration.y == serialBodies[i].acceleration.y);
			CHECK(parallelBodies[i].acceleration.z == serialBodies[i].acceleration.z);
		}
	}
}
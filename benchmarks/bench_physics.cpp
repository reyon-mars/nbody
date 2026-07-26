#include "nbody/physics/body.hpp"
#include "nbody/physics/physics.hpp"
#include <benchmark/benchmark.h>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace
{
	std::vector<nbody::Body> generateRandomBodies(std::size_t count)
	{
		std::mt19937_64 rng(42);
		std::uniform_real_distribution<double> dist(-100.0, 100.0);

		std::vector<nbody::Body> bodies;
		bodies.reserve(count);

		for (std::size_t i = 0; i < count; ++i)
		{
			bodies.push_back(nbody::Body{.position = {dist(rng), dist(rng), dist(rng)},
										 .velocity = {dist(rng), dist(rng), dist(rng)},
										 .mass = dist(rng)});
		}
		return bodies;
	}
} // namespace

static void BM_ComputeAcceleration_Serial(benchmark::State& state)
{
	const auto n = static_cast<std::size_t>(state.range());
	auto bodies = generateRandomBodies(n);
	constexpr double G = 1.0;

	for (auto _ : state)
	{
		nbody::computeAcceleration(bodies, G);
		benchmark::DoNotOptimize(bodies.data());
		benchmark::ClobberMemory();
	}
	state.SetComplexityN(static_cast<int64_t>(n * n));
	state.SetItemsProcessed(static_cast<int64_t>(static_cast<std::size_t>(state.iterations()) * n));
}

BENCHMARK(BM_ComputeAcceleration_Serial)->RangeMultiplier(4)->Range(64, 4096)->Complexity();
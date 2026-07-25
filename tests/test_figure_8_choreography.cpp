#include "catch2/catch_test_macros.hpp"
#include "nbody/body.hpp"
#include "nbody/physics.hpp"
#include "nbody/vector3D.hpp"
#include <cmath>
#include <cstddef>
#include <vector>

#define FIG8TAG "[Figure_8]"

using nbody::Body;
using nbody::Vector3D;

namespace
{
	std::vector<Body> figureEightInitialConditions()
	{
		const double vx3{0.93240737};
		const double vy3{0.86473146};
		return {
			Body{.position = {-0.97000436, 0.24308753, 0.0}, .velocity = {-vx3 / 2.0, -vy3 / 2.0, 0.0}, .mass = 1.0},
			Body{.position = {0.97000436, -0.24308753, 0.0}, .velocity = {-vx3 / 2.0, -vy3 / 2.0, 0.0}, .mass = 1.0},
			Body{.position = {0.0, 0.0, 0.0}, .velocity = {vx3, vy3, 0.0}, .mass = 1.0},
		};
	};

	constexpr double kG{1.0};
	constexpr double kDt{0.0001};
	constexpr int kStepsPerPeriod = 63259; // T ~= 6.3259, measured empirically
} // namespace

TEST_CASE("figure-eight orbit conserves energy to within 0.01% over one period", FIG8TAG)
{
	auto bodies = figureEightInitialConditions();

	nbody::computeAcceleration(bodies, kG);
	const double E0{nbody::totalEnergy(bodies, kG)};

	for (std::size_t i = 0; i < kStepsPerPeriod; ++i)
	{
		nbody::velocityVerletStep(bodies, kDt, kG);
	}

	const double Ef{nbody::totalEnergy(bodies, kG)};
	const double relativeDrift{std::abs(Ef - E0) / std::abs(E0)};

	CHECK(relativeDrift < 1e-4);
}

TEST_CASE("figure-eight orbit returns to within 0.001 of its starting positions after one period", FIG8TAG)
{
	auto bodies = figureEightInitialConditions();

	const std::vector<Vector3D> initialPositions{
		bodies[0].position,
		bodies[1].position,
		bodies[2].position,
	};

	nbody::computeAcceleration(bodies, kG);

	for (std::size_t i = 0; i < kStepsPerPeriod; ++i)
	{
		nbody::velocityVerletStep(bodies, kDt, kG);
	}

	std::size_t i = 0;
	for (const auto& body : bodies)
	{
		const double mismatch{(body.position - initialPositions[i++]).length()};
		CHECK(mismatch < 1e-3);
	}
}

#include "../include/nbody/body.hpp"
#include "../include/nbody/physics.hpp"
#include "../include/nbody/vector3D.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <vector>

#define PHYSICSTAG "[Physics]"

using Catch::Matchers::WithinRel;
using nbody::Body;
using nbody::Vector3D;

constexpr double relative_margin = 1e-5;

TEST_CASE("gravitationalForce output against expected results", PHYSICSTAG)
{
	Body A{.position = {0.0, 0.0, 0.0}, .mass = 2.0};
	Body B{.position = {3.0, 4.0, 0.0}, .mass = 5.0};

	const Vector3D forceOnA = nbody::gravitationalForce(A, B, 1.0);

	REQUIRE_THAT(forceOnA.x, WithinRel(0.24, relative_margin));
	REQUIRE_THAT(forceOnA.y, WithinRel(0.32, relative_margin));
	REQUIRE(forceOnA.z == 0.0);
}

TEST_CASE("Gravity obeys Newton's third law of Motion: equal and opposite", PHYSICSTAG)
{
	Body A{.position = {0.0, 0.0, 0.0}, .mass = 2.0};
	Body B{.position = {3.0, 4.0, 0.0}, .mass = 5.0};

	Vector3D forceOnA = nbody::gravitationalForce(A, B, 1.0);
	Vector3D forceOnB = nbody::gravitationalForce(B, A, 1.0);

	REQUIRE_THAT(forceOnB.x, WithinRel(-forceOnA.x, relative_margin));
	REQUIRE_THAT(forceOnB.y, WithinRel(-forceOnA.y, relative_margin));

	if (forceOnA.z == 0.0 && forceOnB.z == 0.0)
	{
		REQUIRE(forceOnB.z == 0);
	}
	else
	{
		REQUIRE_THAT(forceOnB.z, WithinRel(forceOnA.z, relative_margin));
	}
}

TEST_CASE("netForce with only one other body equals gravitationalForce directly", PHYSICSTAG)
{
	std::vector<Body> bodies{Body{.position = {0.0, 0.0, 0.0}, .mass = 2.0},
							 Body{.position = {3.0, 4.0, 0.0}, .mass = 5.0}};
	const Vector3D net = nbody::netForce(bodies, 0, 1.0);
	const Vector3D direct = nbody::gravitationalForce(bodies[0], bodies[1], 1.0);

	REQUIRE_THAT(net.x, WithinRel(direct.x, relative_margin));
	REQUIRE_THAT(net.y, WithinRel(direct.y, relative_margin));

	if (direct.z == 0.0)
	{
		REQUIRE(net.z == 0.0);
	}
	else
	{
		REQUIRE_THAT(net.z, WithinRel(direct.z, relative_margin));
	}
}

TEST_CASE("Single Euler step against expected values", PHYSICSTAG)
{
	std::vector<Body> bodies{Body{.position = {0.0, 0.0, 0.0}, .mass = 2.0},
							 Body{.position = {3.0, 4.0, 0.0}, .mass = 5.0}};

	nbody::EulerStep(bodies, 1.0, 1.0);

	REQUIRE_THAT(bodies[0].acceleration.x, WithinRel(0.12, relative_margin));
	REQUIRE_THAT(bodies[0].acceleration.y, WithinRel(0.16, relative_margin));

	REQUIRE_THAT(bodies[0].velocity.x, WithinRel(0.12, relative_margin));
	REQUIRE_THAT(bodies[0].velocity.y, WithinRel(0.16, relative_margin));
	REQUIRE_THAT(bodies[0].position.x, WithinRel(0.12, relative_margin));
	REQUIRE_THAT(bodies[0].position.y, WithinRel(0.16, relative_margin));

	REQUIRE_THAT(bodies[1].acceleration.x, WithinRel(-0.048, relative_margin));
	REQUIRE_THAT(bodies[1].acceleration.y, WithinRel(-0.064, relative_margin));
	REQUIRE_THAT(bodies[1].position.x, WithinRel(2.952, relative_margin));
	REQUIRE_THAT(bodies[1].position.y, WithinRel(3.936, relative_margin));
}

TEST_CASE("Euler's integrator measurably leaks energy ", PHYSICSTAG)
{
	std::vector<Body> bodies = {Body{.position = {0.0, 0.0, 0.0}, .velocity = {0.0, 0.0, 0.0}, .mass = 1000.0},
								Body{.position = {10.0, 0.0, 0.0}, .velocity = {0.0, 10.0, 0.0}, .mass = 1.0}};
	const double G = 1.0;
	const double dt = 0.2;

	const double E0 = nbody::totalEnergy(bodies, G);
	REQUIRE(E0 == Catch::Approx(-50.0));

	const int steps = 620;
	for (int i = 0; i < steps; ++i)
	{
		nbody::EulerStep(bodies, dt, G);
	}

	const double Ef = nbody::totalEnergy(bodies, G);
	const double relativeDrift = std::abs(Ef - E0) / std::abs(E0);
	CHECK(relativeDrift > 0.02);
}
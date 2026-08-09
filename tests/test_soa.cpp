#include "catch2/catch_test_macros.hpp"
#include "nbody/core/system_soa.hpp"
#include "nbody/physics/body.hpp"
#include <vector>

#define SOA_TAG "[soa]"

using nbody::Body;
using nbody::BodySystemSoA;

TEST_CASE("BodySystemSoA round-trips every field from the AoS representation exactly", SOA_TAG)
{
	const std::vector<Body> bodies{
		Body{.position = {1.0, 2.0, 3.0}, .velocity = {4.0, 5.0, 6.0}, .mass = 7.0},
		Body{.position = {-1.5, 0.0, 100.0}, .velocity = {0.0, -2.5, 3.0}, .mass = 0.5},
		Body{.position = {0.0, 0.0, 0.0}, .velocity = {0.0, 0.0, 0.0}, .mass = 1.0},
	};

	const BodySystemSoA soa{bodies};

	REQUIRE(bodies.size() == soa.size());
	std::size_t n = bodies.size();
	for (std::size_t i = 0; i < n; ++i)
	{
		CHECK(soa.x[i] == bodies[i].position.x);
		CHECK(soa.y[i] == bodies[i].position.y);
		CHECK(soa.z[i] == bodies[i].position.z);
		CHECK(soa.vx[i] == bodies[i].velocity.x);
		CHECK(soa.vy[i] == bodies[i].velocity.y);
		CHECK(soa.vz[i] == bodies[i].velocity.z);
		CHECK(soa.mass[i] == bodies[i].mass);
	}
}

TEST_CASE("BodySystemSoA handles the empty case", SOA_TAG)
{
	const std::vector<Body> bodies;
	const BodySystemSoA soa{bodies};

	CHECK(soa.size() == 0);
}

#include "../include/vector3D.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

#define VECTOR3DTAG

using Catch::Matchers::WithinAbs;
using Catch::Matchers::WithinRel;
using nbody::Vector3D;

TEST_CASE("Vector substraction gives the displacement between the two points", VECTOR3DTAG)
{
	const Vector3D r_i{10.0, 4.0, 0.0};
	const Vector3D r_j{2.0, 1.0, 3.0};

	const Vector3D diff = r_i - r_j;

	REQUIRE_THAT(diff.x, WithinRel(8.0, 0.0001));
	REQUIRE_THAT(diff.y, WithinRel(3.0, 0.0001));
	REQUIRE_THAT(diff.z, WithinRel(-3.0, 0.0001));
}

TEST_CASE("length() computes the 3D Pythagorean", VECTOR3DTAG)
{
	const Vector3D diff{8.0, 3.0, -3.0};
	REQUIRE_THAT(diff.length(), WithinRel(std::sqrt(82.0), 0.0001));
}

TEST_CASE("scalar multiplication followed by vector addition", VECTOR3DTAG)
{
	const Vector3D v{2.0, -3.0, 1.0};
	const Vector3D w{-1.0, 4.0, 2.0};

	const Vector3D result = 3.0 * v + w;

	REQUIRE_THAT(result.x, WithinRel(5.0, 0.0001));
	REQUIRE_THAT(result.y, WithinRel(-5.0, 0.0001));
	REQUIRE_THAT(result.z, WithinRel(5.0, 0.0001));
}

TEST_CASE("normalized() preserves direction and yields length 1", VECTOR3DTAG)
{
	const Vector3D v{3.0, 4.0, 0.0};
	const Vector3D u = v.normalized();

	REQUIRE_THAT(u.x, WithinRel(0.6, 0.0001));
	REQUIRE_THAT(u.y, WithinRel(0.8, 0.0001));
	REQUIRE_THAT(v.z, WithinRel(0.0, 0.0001));
	REQUIRE_THAT(u.length(), WithinRel(1.0, 0.0001));
}

TEST_CASE("vector addition is commutative, tip to tail either order", VECTOR3DTAG)
{
	const Vector3D p{4.0, 1.0, 0.0};
	const Vector3D q{1.0, 3.0, 0.0};

	const Vector3D sum1 = p + q;
	const Vector3D sum2 = q + p;

	REQUIRE_THAT(sum1.x, WithinRel(5.0, 0.0001));
	REQUIRE_THAT(sum1.y, WithinRel(4.0, 0.0001));
	REQUIRE_THAT(sum1.x, WithinRel(sum2.x, 0.0001));
	REQUIRE_THAT(sum1.y, WithinRel(sum2.y, 0.0001));
}
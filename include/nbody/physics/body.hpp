#pragma once

#include "nbody/core/vector3D.hpp"

namespace nbody
{
	/* *********************************************************************
     * Body represents a single point-mass particle within the N-body simulation
     * having position, velocity acceleration and mass. 
     * *********************************************************************/
	struct Body
	{
		Vector3D position{};	 // Current position in the 3D space
		Vector3D velocity{};	 // Velocity vector
		Vector3D acceleration{}; // Acceleration vector
		double mass{1.0};		 // Body mass
	};
} // namespace nbody
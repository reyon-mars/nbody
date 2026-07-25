#pragma once

#include "nbody/body.hpp"
#include "nbody/physics.hpp"
#include "nbody/vector3D.hpp"
#include <algorithm>
#include <cstddef>
#include <thread>
#include <vector>

namespace nbody
{
	inline void computeAccelerationParallel(std::vector<Body>& bodies, double G, std::size_t numThreads)
	{
		const std::size_t n = bodies.size();
		const std::size_t threadsToUse = std::max<std::size_t>(1, std::min(numThreads, n));
		const std::size_t chunkSize = (n + threadsToUse - 1) / threadsToUse;

		std::vector<std::thread> workers;
		workers.reserve(threadsToUse);

		for (std::size_t t = 0; t < threadsToUse; ++t)
		{
			const std::size_t begin = t * chunkSize;
			const std::size_t end = std::min((begin + chunkSize), n);

			if (begin >= end)
			{
				continue;
			}

			workers.emplace_back(
				[&bodies, begin, end, G]()
				{
					for (std::size_t i = begin; i < end; ++i)
					{
						bodies[i].acceleration = netForce(bodies, i, G) / bodies[i].mass;
					}
				});
			for (auto& worker : workers)
			{
				worker.join();
			}
		}
	}
} // namespace nbody
#pragma once

#include <cstdlib>
#include <ctime>

namespace Utilities {
	namespace Random {

		void seedRandom(unsigned int seed = time(NULL));

		int randBetweenInt(const int &min, const int &max);

		int randInt();

		float randBetweenFloat(const float &min, const float &max);

		float randFloat();

	}
}
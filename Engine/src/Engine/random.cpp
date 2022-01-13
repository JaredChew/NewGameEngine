#include "random.h"

void Utilities::Random::seedRandom(unsigned int seed) {

	static bool isSeeded = false;

	if (!isSeeded) {
		isSeeded = true;
		srand(seed);
	}

}

int Utilities::Random::randBetweenInt(const int &min, const int &max) {

	/*

		Code reference

		int output = min + (rand() % static_cast<int>(max - min + 1))

		https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range


	*/

	return min + (rand() % ((max - min) + 1));

}

int Utilities::Random::randInt() {

	return rand();

}

float Utilities::Random::randBetweenFloat(const float &min, const float &max) {

	/*

		Code reference

		float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

		https://stackoverflow.com/questions/686353/random-float-number-generation/17798317
		https://stackoverflow.com/questions/4310277/producing-random-float-from-negative-to-positive-range

	*/

	//float rng = fmodf(rand(), notMore); //inside a do-while(rng < startNum)

	//return min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
	return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;

}

float Utilities::Random::randFloat() {

	return float(rand()) / float(RAND_MAX);

}
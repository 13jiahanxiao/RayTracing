#ifndef RANDOMH
#define RANDOMH
#include <random>
#include <functional>


double RandomDouble()
{
	/*static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	static std::function<double()> randGenerator = std::bind(distribution, generator);
	return randGenerator();*/
	return rand() / (RAND_MAX + 1.0);
}

Vector3 RandomFlect()
{
	Vector3 p;
	do {
		p = 2.0 * Vector3(RandomDouble(), RandomDouble(), RandomDouble()) - Vector3(1, 1, 1);
	} while (p.SquaredLength() >= 1);
	return p;
}
Vector3 RandomInUnitDisk() {
	Vector3 p;
	do {
		p = 2.0 * Vector3(RandomDouble(), RandomDouble(), 0) - Vector3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

#endif

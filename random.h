#ifndef RANDOMH
#define RANDOMH
#include <random>
#include <functional>


double RandomDouble()
{
	static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	static std::function<double()> randGenerator = std::bind(distribution, generator);
	return randGenerator();
}

Vec3 RandomFlect()
{
	Vec3 p;
	do {
		p = 2.0 * Vec3(RandomDouble(), RandomDouble(), RandomDouble()) - Vec3(1, 1, 1);
	} while (p.SquaredLength() >= 1);
	return p;
}
Vec3 RandomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(RandomDouble(), RandomDouble(), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

#endif

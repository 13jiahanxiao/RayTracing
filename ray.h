#ifndef RAYH
#define RAYH

#include"vector.h"

class Ray
{
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b, float t = 0) { A = a; B = b; time = 0; }
	float Time() const{ return time; }
	Vec3 Origin() const { return A; }
	Vec3 Direction() const { return B; }
	Vec3 PointAtParameter(float t)const { return A + t * B; }

	Vec3 A;
	Vec3 B;
	float time;
};
#endif

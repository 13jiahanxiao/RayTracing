#ifndef RAYH
#define RAYH

#include"vector.h"

class Ray
{
public:
	Ray() {}
	Ray(const Vector3& a, const Vector3& b, float t = 0) { A = a; B = b; time = 0; }
	float Time() const{ return time; }
	Vector3 Origin() const { return A; }
	Vector3 Direction() const { return B; }
	Vector3 PointAtParameter(float t)const { return A + t * B; }

	Vector3 A;
	Vector3 B;
	float time;
};
#endif

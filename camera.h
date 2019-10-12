#ifndef CAMERAH
#define CAMERAH

#include"vector.h"
#include"ray.h"

class Camera
{
public:
	Camera()
	{
		lowerLeftCorner = Vec3(-8, -4.5f, -3);
		horizontal = Vec3(16, 0, 0);
		vertical = Vec3(0, 9, 0);
		origin = Vec3(0, 0, 0);
	}
	Ray GetRay(float u, float v) { return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical-origin); }
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;

};
#endif
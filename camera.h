#ifndef CAMERAH
#define CAMERAH

#include"vector.h"
#include"ray.h"
#include"random.h"
#define M_PI 3.1415926

class Camera
{
public:
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect,float aperture,float focusDistance) {

		lensRadius = aperture / 2;
		float theta = vfov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		origin = lookfrom;
		w = UnitVector(lookfrom - lookat);
		u = UnitVector(cross(vup, w));
		v = cross(w, u);
		lowerLeftCorner = origin - halfWidth * u*focusDistance - halfHeight * v*focusDistance - w*focusDistance;
		horizontal = 2 * focusDistance*halfWidth * u;
		vertical = 2 * focusDistance*halfHeight * v;
	}

	Ray GetRay(float s, float t)
	{
		Vec3 rd = lensRadius * RandomInUnitDisk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin+offset,  UnitVector (lowerLeftCorner + s * horizontal + t * vertical-origin-offset)); 
	}
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;
	Vec3 u, v, w;
	float lensRadius;
};
#endif
#ifndef CAMERAH
#define CAMERAH

#include"vector.h"
#include"ray.h"
#include"random.h"

class Camera
{
public:
	Camera();
	Camera(Vector3 lookfrom, Vector3 lookat, Vector3 vup, float vfov, float aspect,float aperture,float focusDistance,float t0,float t1) {

		time0 = t0;
		time1 = t1;
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
		Vector3 rd = lensRadius * RandomInUnitDisk();
		Vector3 offset = u * rd.x() + v * rd.y();
		float time = time0 + RandomDouble() * (time1 - time0);
		return Ray(origin+offset,  UnitVector (lowerLeftCorner + s * horizontal + t * vertical-origin-offset),time); 
	}
	Vector3 lowerLeftCorner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 origin;
	Vector3 u, v, w;
	float lensRadius;
	float time0, time1;
};
#endif
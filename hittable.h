#ifndef HITTABLEH
#define HITTABLEH

#include"vector.h"
#include"ray.h"

class Material;

struct  HitRecord
{
	float t;
	Vec3 p;
	Vec3 normal;
	Material *material;
};
class Hittable
{
public:
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const = 0;
};

#endif

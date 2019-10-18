#ifndef HITTABLEH
#define HITTABLEH

#include"vector.h"
#include"ray.h"
#include"bvh.h"

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
	virtual bool BoundingBox(float t0, float t1, AABB& r)const = 0;
};

#endif
